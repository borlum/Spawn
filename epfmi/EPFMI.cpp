#include "EPFMI.hpp"
#include "Variables.hpp"
#include "EPComponent.hpp"
#include <fmi2Functions.h>
#include "EnergyPlus/api/EnergyPlusPgm.hh"
#include "EnergyPlus/CommandLineInterface.hh"
#include "EnergyPlus/ZoneTempPredictorCorrector.hh"
#include "EnergyPlus/DataGlobals.hh"
#include "EnergyPlus/DataHeatBalFanSys.hh"
#include "EnergyPlus/DataHeatBalance.hh"
#include "EnergyPlus/DataEnvironment.hh"
#include "EnergyPlus/DataRuntimeLanguage.hh"
#include "EnergyPlus/OutputProcessor.hh"
#include "EnergyPlus/EMSManager.hh"
#include "EnergyPlus/SimulationManager.hh"
#include "EnergyPlus/UtilityRoutines.hh"
#include "EnergyPlus/InputProcessing/IdfParser.hh"
#include "EnergyPlus/InputProcessing/EmbeddedEpJSONSchema.hh"
#include "EnergyPlus/api/runtime.h"
#include <boost/filesystem.hpp>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <algorithm>
#include <list>
#include <regex>

using namespace std::placeholders;

#define UNUSED(expr) do { (void)(expr); } while (0);


bool done = false;
void doAfterInitHeatBalance(EPComponent * epcomp) {
  std::cout << "doAfterInitHeatBalance !!" << std::endl;
  if (! done) {
    std::cout << "doAfterInitHeatBalance" << std::endl;
    EnergyPlus::DataHeatBalance::SNLoadHeatRate.dimension(EnergyPlus::DataGlobals::NumOfZones, 0.0);
    done = true;
    //EnergyPlus::DataHeatBalance::MaxHeatLoadZone.dimension(EnergyPlus::DataGlobals::NumOfZones, 0.0);
  }
}

void doEndOfSystemTimeStepAfterHVACReporting(EPComponent * epcomp) {
  // At this time, there is no data exchange or any other
  // interaction with the FMU during wramup and sizing.
  if( EnergyPlus::DataGlobals::DoingSizing || EnergyPlus::DataGlobals::WarmupFlag ) {
    return;
  }

  // Only signal and wait for input if the current sim time is greather than or equal
  // to the requested time
  if( epcomp->currentSimTime() < epcomp->requestedTime ) {
    // Might want to exchange data here, so that EnergyPlus
    // does not overrite any fmu inputs
    return;
  }

  // Signal the end of the step
  {
    if (epcomp->epstatus != EPStatus::TERMINATE)
    {
      std::unique_lock<std::mutex> lk(epcomp->control_mutex);
      epcomp->epstatus = EPStatus::NONE;
    }
  }
  epcomp->control_cv.notify_one();

  // Wait for the epstatus to advance again
  std::unique_lock<std::mutex> lk(epcomp->control_mutex);
  epcomp->control_cv.wait(lk, [&epcomp]() {
    return (
      (epcomp->epstatus == EPStatus::ADVANCE) ||
      (epcomp->epstatus == EPStatus::TERMINATE)
    );
  });
}

std::list<EPComponent> epComponents;

void exchange(EPComponent * epcomp)
{
  auto zoneNum = [](std::string & zoneName) {
    std::transform(zoneName.begin(), zoneName.end(), zoneName.begin(), ::toupper);
    for ( int i = 0; i < EnergyPlus::DataGlobals::NumOfZones; ++i ) {
      if ( EnergyPlus::DataHeatBalance::Zone[i].Name == zoneName ) {
        return i + 1;
      }
    }

    return 0;
  };

  auto getSensorValue = [&](Variable & var) {
    int varType;
    int varIndex;

    EnergyPlus::EMSManager::GetVariableTypeAndIndex(var.epname, var.epkey, varType, varIndex);
    return EnergyPlus::GetInternalVariableValue(varType, varIndex);
  };

  auto setActuatorValue = [](std::string & actuatorName, const Real64 & value) {
    std::transform(actuatorName.begin(), actuatorName.end(), actuatorName.begin(), ::toupper);

    for ( int i = 0; i < EnergyPlus::DataRuntimeLanguage::numActuatorsUsed; ++i ) {
      auto & used = EnergyPlus::DataRuntimeLanguage::EMSActuatorUsed[i];
      if (used.Name == actuatorName) {
        auto & actuator = EnergyPlus::DataRuntimeLanguage::EMSActuatorAvailable(used.ActuatorVariableNum);
        actuator.RealValue = value;
        actuator.Actuated = true;
      }
    }
  };

  for( auto & varmap : epcomp->variables ) {
    auto & var = varmap.second;
    auto varZoneNum = zoneNum(var.key);
    switch ( var.type ) {
      //case VariableType::T:
      //  EnergyPlus::DataHeatBalFanSys::ZT( varZoneNum ) = var.value;
      //  EnergyPlus::DataHeatBalFanSys::MAT( varZoneNum ) = var.value;
      //  break;
      case VariableType::V:
        var.value = EnergyPlus::DataHeatBalance::Zone( varZoneNum ).Volume;
        break;
      case VariableType::AFLO:
        var.value = EnergyPlus::DataHeatBalance::Zone( varZoneNum ).FloorArea;
        break;
      case VariableType::MSENFAC:
        var.value = EnergyPlus::DataHeatBalance::Zone( varZoneNum ).ZoneVolCapMultpSens;
        break;
      case VariableType::QCONSEN_FLOW:
        //var.value = EnergyPlus::ZoneTempPredictorCorrector::HDot( varZoneNum );
        break;
      case VariableType::SENSOR:
        var.value = getSensorValue(var);
        break;
      case VariableType::EMS_ACTUATOR:
        setActuatorValue(var.key, var.value);
        break;
      default:
        break;
    }
  }
}

EPFMI_API fmi2Component fmi2Instantiate(fmi2String instanceName,
  fmi2Type fmuType,
  fmi2String fmuGUID,
  fmi2String fmuResourceURI,
  const fmi2CallbackFunctions* functions,
  fmi2Boolean visible,
  fmi2Boolean loggingOn)
{
  UNUSED(fmuType);
  UNUSED(fmuGUID);
  UNUSED(visible);

  epComponents.emplace_back();
  auto & comp = epComponents.back();
  comp.instanceName = instanceName;
  comp.logger = functions->logger;
  comp.loggingOn = loggingOn;

	const auto resourcePathString = std::regex_replace(fmuResourceURI, std::regex("^file://"), "");
  const auto resourcePath = boost::filesystem::path(resourcePathString);

  boost::system::error_code ec;
  for ( const auto & entry : boost::filesystem::directory_iterator(resourcePath, ec) ) {
    const auto path = entry.path();
    const auto extension = path.extension();
    if ( extension == ".idf" ) {
      comp.idfInputPath = path.string();
    } else if ( extension == ".epw" ) {
      comp.weatherFilePath = path.string();
    } else if ( extension == ".idd" ) {
      comp.iddPath = path.string();
    } else if ( extension == ".spawn" ) {
      comp.spawnInputPath = path.string();
    } else if ( extension == ".json" ) {
      comp.spawnInputPath = path.string();
    }
  }

  comp.variables = parseVariables(comp.idfInputPath, comp.spawnInputPath);

  return &comp;
}

EPFMI_API fmi2Status fmi2SetupExperiment(fmi2Component c,
  fmi2Boolean toleranceDefined,
  fmi2Real tolerance,
  fmi2Real startTime,
  fmi2Boolean stopTimeDefined,
  fmi2Real stopTime)
{
  EPComponent * epcomp = static_cast<EPComponent*>(c);

  epcomp->toleranceDefined = toleranceDefined;
  epcomp->tolerance = tolerance;
  epcomp->startTime = startTime;
  epcomp->stopTimeDefined = stopTimeDefined;
  epcomp->stopTime = stopTime;

  auto simulation = [&epcomp](){
    auto workingPath = boost::filesystem::path(epcomp->instanceName).filename();

    const int argc = 8;
    const char * argv[argc];
    argv[0] = "energyplus";
    argv[1] = "-d";
    argv[2] = workingPath.string().c_str();
    argv[3] = "-w";
    argv[4] = epcomp->weatherFilePath.c_str();
    argv[5] = "-i";
    argv[6] = epcomp->iddPath.c_str();
    argv[7] = epcomp->idfInputPath.c_str();

    EnergyPlus::CommandLineInterface::ProcessArgs( argc, argv );

    std::cout << "register callback" << std::endl;
    //const auto & f = std::bind(doEndOfSystemTimeStepAfterHVACReporting, epcomp);
    callbackEndOfSystemTimeStepAfterHVACReporting(std::bind(doEndOfSystemTimeStepAfterHVACReporting, epcomp));
    //emsCallFromBeginZoneTimestepAfterInitHeatBalance
    //      BeginZoneTimestepAfterInitHeatBalance
    callbackBeginZoneTimeStepAfterInitHeatBalance(std::bind(doAfterInitHeatBalance, epcomp));
    //callbackBeginNewEnvironment(doBeginNewEnvironment);
    //std::cout << "numberofZones: " << EnergyPlus::DataGlobals::NumOfZones << std::endl;
    //EnergyPlus::DataHeatBalance::SNLoadHeatRate.dimension(EnergyPlus::DataGlobals::NumOfZones, 0.0);

    RunEnergyPlus();
  };

  {
    std::unique_lock<std::mutex> lk(epcomp->control_mutex);
    epcomp->requestedTime = 0.0;
    epcomp->epstatus = EPStatus::ADVANCE;
  }

  epcomp->simthread = std::thread(simulation);

  {
    // Wait for E+ to go back to IDLE
    std::unique_lock<std::mutex> lk( epcomp->control_mutex );
    epcomp->control_cv.wait( lk, [&epcomp](){ return epcomp->epstatus == EPStatus::NONE; } );
  }

  return fmi2OK;
}

EPFMI_API fmi2Status fmi2SetTime(fmi2Component c, fmi2Real time)
{
  EPComponent * epcomp = static_cast<EPComponent*>(c);
  epcomp->requestedTime = time;

  exchange(epcomp);

  {
    std::unique_lock<std::mutex> lk(epcomp->control_mutex);
    epcomp->epstatus = EPStatus::ADVANCE;
  }
  // Notify E+ to advance
  epcomp->control_cv.notify_one();
  {
    // Wait for E+ to advance and go back to IDLE before returning
    std::unique_lock<std::mutex> lk( epcomp->control_mutex );
    epcomp->control_cv.wait( lk, [&epcomp](){ return epcomp->epstatus == EPStatus::NONE; } );
  }

  return fmi2OK;
}

EPFMI_API fmi2Status fmi2SetReal(fmi2Component c,
  const fmi2ValueReference vr[],
  size_t nvr,
  const fmi2Real values[])
{
  EPComponent * epcomp = static_cast<EPComponent*>(c);

  for ( size_t i = 0; i < nvr; ++i ) {
    auto valueRef = vr[i];
    auto value = values[i];
    auto varit = epcomp->variables.find(valueRef);
    if ( varit != epcomp->variables.end() ) {
      varit->second.value = value;
    }
  }

  return fmi2OK;
}

EPFMI_API fmi2Status fmi2GetReal(fmi2Component c,
  const fmi2ValueReference vr[],
  size_t nvr,
  fmi2Real values[])
{
  EPComponent * epcomp = static_cast<EPComponent*>(c);

  exchange(epcomp);

  for ( size_t i = 0; i < nvr; ++i ) {
    auto valueRef = vr[i];
    auto varit = epcomp->variables.find(valueRef);
    if ( varit != epcomp->variables.end() ) {
      values[i] = varit->second.value;
    }
  }

  return fmi2OK;
}

EPFMI_API fmi2Status fmi2NewDiscreteStates(fmi2Component  c, fmi2EventInfo* eventInfo)
{
  EPComponent * epcomp = static_cast<EPComponent*>(c);

  eventInfo->newDiscreteStatesNeeded = fmi2False;
  eventInfo->nextEventTime = epcomp->nextSimTime();
  eventInfo->nextEventTimeDefined = fmi2True;
  eventInfo->terminateSimulation = fmi2False;

  return fmi2OK;
}

EPFMI_API fmi2Status fmi2Terminate(fmi2Component c)
{
  EPComponent * epcomp = static_cast<EPComponent*>(c);

  {
    std::unique_lock<std::mutex> lk(epcomp->control_mutex);
    epcomp->epstatus = EPStatus::TERMINATE;
  }

  stopSimulation();
  epcomp->control_cv.notify_one();
  epcomp->simthread.join();

  auto it = std::find(epComponents.begin(), epComponents.end(), *epcomp);
  epComponents.erase(it);

  return fmi2OK;
}

EPFMI_API const char* fmi2GetTypesPlatform(void)
{
  return fmi2TypesPlatform;
}

EPFMI_API const char* fmi2GetVersion(void)
{
  return fmi2Version;
}

EPFMI_API fmi2Status fmi2SetDebugLogging(fmi2Component, fmi2Boolean, size_t, const fmi2String[])
{
  return fmi2OK;
}

EPFMI_API fmi2Status fmi2Reset(fmi2Component c)
{
  EPComponent * epcomp = static_cast<EPComponent*>(c);
  epcomp->variables = parseVariables(epcomp->idfInputPath,epcomp->spawnInputPath);

  return fmi2OK;
}

EPFMI_API void fmi2FreeInstance(fmi2Component c)
{
  EPComponent * epcomp = static_cast<EPComponent*>(c);

  auto it = std::find(epComponents.begin(), epComponents.end(), *epcomp);
  epComponents.erase(it);

  c = nullptr;
}

EPFMI_API fmi2Status fmi2EnterInitializationMode(fmi2Component)
{
  return fmi2OK;
}

EPFMI_API fmi2Status fmi2ExitInitializationMode(fmi2Component)
{
  return fmi2OK;
}

EPFMI_API fmi2Status fmi2GetInteger(fmi2Component, const fmi2ValueReference[], size_t, fmi2Integer[])
{
  return fmi2OK;
}

EPFMI_API fmi2Status fmi2GetBoolean(fmi2Component, const fmi2ValueReference[], size_t, fmi2Boolean[])
{
  return fmi2OK;
}

EPFMI_API fmi2Status fmi2GetString(fmi2Component, const fmi2ValueReference[], size_t, fmi2String [])
{
  return fmi2OK;
}

EPFMI_API fmi2Status fmi2SetInteger(fmi2Component, const fmi2ValueReference[], size_t, const fmi2Integer[])
{
  return fmi2OK;
}

EPFMI_API fmi2Status fmi2SetBoolean(fmi2Component, const fmi2ValueReference[], size_t, const fmi2Boolean[])
{
  return fmi2OK;
}

EPFMI_API fmi2Status fmi2SetString(fmi2Component, const fmi2ValueReference[], size_t, const fmi2String [])
{
  return fmi2OK;
}

EPFMI_API fmi2Status fmi2EnterEventMode(fmi2Component)
{
  return fmi2OK;
}


EPFMI_API fmi2Status fmi2EnterContinuousTimeMode(fmi2Component)
{
  return fmi2OK;
}

EPFMI_API fmi2Status fmi2CompletedIntegratorStep(fmi2Component, fmi2Boolean, fmi2Boolean* enterEventMode, fmi2Boolean* terminateSimulation)
{
  // TODO: What happens if we get to the end of the run period?
  // Consider setting terminateSimulation to true
  *terminateSimulation = fmi2False;
  *enterEventMode = fmi2False;

  return fmi2OK;
}

EPFMI_API fmi2Status fmi2SetContinuousStates(fmi2Component, const fmi2Real[], size_t)
{
  return fmi2OK;
}

EPFMI_API fmi2Status fmi2GetDerivatives(fmi2Component, fmi2Real[], size_t)
{
  return fmi2OK;
}

EPFMI_API fmi2Status fmi2GetEventIndicators(fmi2Component, fmi2Real[], size_t)
{
  return fmi2OK;
}

EPFMI_API fmi2Status fmi2GetContinuousStates(fmi2Component, fmi2Real[], size_t)
{
  return fmi2OK;
}

EPFMI_API fmi2Status fmi2GetNominalsOfContinuousStates(fmi2Component, fmi2Real[], size_t)
{
  return fmi2OK;
}


