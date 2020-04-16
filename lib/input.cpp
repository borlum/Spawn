#include "input.hpp"
#include "../util/idf_to_json.hpp"

using json = nlohmann::json;

namespace spawn {

Input::Input(const std::string & spawninput)
{
  std::ifstream fileinput(spawninput);
  if (!fileinput.fail()) {
    // deserialize from file
    fileinput >> spawnjson;
    m_basepath = boost::filesystem::canonical(boost::filesystem::path(spawninput)).parent_path();
  } else {
    // Try to parse command line input as json string
    spawnjson = json::parse(spawninput, nullptr, false);
    m_basepath = boost::filesystem::current_path();
  }

  json weather;

  if (spawnjson.is_discarded()) {
    std::cout << "Cannot parse json: '" << spawninput << "'" << std::endl;
  }

  // Do the input paths exist?
  if (! boost::filesystem::exists(idfInputPath())) {
    std::cout << "The specified idf input file does not exist, " << idfInputPath() << "." << std::endl;
  }
  if (! boost::filesystem::exists(epwInputPath())) {
    std::cout << "The specified epw input file does not exist, " << epwInputPath() << "." << std::endl;
  }

  jsonidf = idfToJSON(idfInputPath());

  zones = Zone::createZones(spawnjson, jsonidf);
  schedules = Schedule::createSchedules(spawnjson, jsonidf);
  outputVariables = OutputVariable::createOutputVariables(spawnjson, jsonidf);
  emsActuators = EMSActuator::createEMSActuators(spawnjson, jsonidf);
}

std::string Input::fmuname() const {
  return spawnjson["fmu"].value("name","spawn.fmu");
}

std::string Input::fmuBaseName() const {
  return boost::filesystem::path(fmuname()).stem().string();
}

void Input::setFMUName(const std::string & name) {
  spawnjson["fmu"]["name"] = name;
}

boost::filesystem::path Input::toPath(const std::string & pathstring) const {
  boost::filesystem::path p(pathstring);
  if (! p.is_absolute()) {
    p = m_basepath / p;
  }

  return p;
}

boost::filesystem::path Input::idfInputPath() const {
  return toPath(spawnjson["EnergyPlus"].value("idf",""));
}

void Input::setIdfInputPath(boost::filesystem::path idfpath) {
  spawnjson["EnergyPlus"]["idf"] = idfpath.string();
}

boost::filesystem::path Input::epwInputPath() const {
  return toPath(spawnjson["EnergyPlus"].value("weather",""));
}

void Input::setEPWInputPath(boost::filesystem::path epwpath) {
  spawnjson["EnergyPlus"]["weather"] = epwpath.string();
}

void Input::save(const boost::filesystem::path & savepath) const {
  std::ofstream o(savepath.string());
  o << std::setw(4) << spawnjson << std::endl;
}

boost::filesystem::path Input::basepath() const {
  return m_basepath;
}

} // namespace spawn

