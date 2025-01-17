#include "../fmu/fmi2.hpp"
#include "../fmu/fmu.hpp"
#include "../fmu/logger.h"
#include "../fmu/modeldescription.hpp"
#include "../util/config.hpp"
#include "../util/filesystem.hpp"
#include "../util/math.hpp"
#include "paths.hpp"
#include <catch2/catch.hpp>

#if defined ENABLE_MODELICA_COMPILER

TEST_CASE("Test Zone Multiplier")
{
  const std::string model_name = "Zone_Multiplier";
  const std::string fmu_name = "Zone_Multiplier.fmu";
  const auto model_dir = spawn::project_binary_dir() / "test/models/Zone_Multiplier.mo";

  const auto cmd =
      spawnexe() + " modelica --modelica-path " + model_dir.string() + " --create-fmu " + model_name + " --fmu-type ME";
  const auto result = system(cmd.c_str()); // NOLINT
  REQUIRE(result == 0);

  const auto fmu_path = spawn::project_binary_dir() / fmu_name;
  spawn::fmu::FMU fmu{fmu_path, false};

  CHECK(fmu.fmi.fmi2GetVersion() == std::string("2.0"));
  const auto resource_path = std::string("file://") + (fmu.extractedFilesPath() / "resources").string();

  spawn::fmu::ModelDescription modelDescription(fmu.extractedFilesPath() / fmu.modelDescriptionPath());

  fmi2CallbackFunctions callbacks = {
      fmuStdOutLogger, calloc, free, nullptr, nullptr}; // called by the model during simulation
  const auto comp = fmu.fmi.fmi2Instantiate("test-instance",
                                            fmi2ModelExchange,
                                            modelDescription.guid().c_str(),
                                            resource_path.c_str(),
                                            &callbacks,
                                            false,
                                            true);

  // Run the model for a day
  fmi2Status status = fmu.fmi.fmi2SetupExperiment(comp, false, 0.0, 0.0, false, 0.0);
  CHECK(status == fmi2OK);
  status = fmu.fmi.fmi2EnterInitializationMode(comp);
  CHECK(status == fmi2Error);
}

#endif // ENABLE_MODELICA_COMPILER
