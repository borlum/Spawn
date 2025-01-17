#include "compile.hpp"
#include "../compiler/compiler.hpp"
#include "../fmu/modeldescription.hpp"
#include "../lib/ziputil.hpp"
#include "../util/config.hpp"
#include "cli/embedded_files.hxx"
#include "jmodelica.h"
#include "optimica.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <pugixml.hpp>
#include <spdlog/spdlog.h>

using json = nlohmann::json;

namespace spawn {

const char *toString(FMUType t)
{
  switch (t) {
  case FMUType::ME:
    return "ME";
  case FMUType::CS:
    return "CS";
  default:
    return "CS";
  }
}

FMUType toFMUType(const std::string &t)
{
  if (t == "ME") {
    return FMUType::ME;
  } else if (t == "CS") {
    return FMUType::CS;
  } else {
    return FMUType::CS;
  }
}

// Given a vector of file paths, return any path that appears to be a path
// to the MBL, if not found return empty path
spawn_fs::path mblPathInPaths(const std::vector<std::string> &modelicaPaths)
{
  for (const auto &p : modelicaPaths) {
    auto path = spawn_fs::path(p);
    const auto package_path = path / "package.mo";
    const auto name = package_path.parent_path().stem().generic_string();
    if (spawn_fs::exists(package_path) && (name == "Buildings")) {
      return path;
    }
  }

  return spawn_fs::path();
}

// Convert a vector of paths to a colon deliminated path string
std::string pathVectorToPath(const std::vector<std::string> &paths)
{
  std::stringstream ss;
  std::ostream_iterator<std::string> it(ss, ":");
  std::copy(paths.begin(), paths.end(), it);
  return ss.str();
}

// Convert a colon deliminated path string to a vector of paths
std::vector<std::string> pathToPathVector(const std::string &path)
{
  std::stringstream ss(path);
  std::vector<std::string> result;
  std::string s;
  while (std::getline(ss, s, ':')) {
    result.push_back(s);
  }
  return result;
}

// Return the mbl path from the environment variable
spawn_fs::path mblPathFromEnv()
{
  const std::string pathstring = getenv("MODELICAPATH");
  const auto pathvector = pathToPathVector(pathstring);
  return mblPathInPaths(pathvector);
}

std::vector<spawn_fs::path> includePaths(const spawn_fs::path &jmodelica_dir,
                                         const spawn_fs::path &embedded_files_temp_dir)
{
  // const std::string mbl_energyplus_c_sources = "Resources/src/ThermalZones/" + spawn::mbl_energyplus_version_string()
  // + "/C-Sources/";
  const auto mbl_path = mblPathFromEnv();
  std::vector<spawn_fs::path> result = {jmodelica_dir / "include/RuntimeLibrary/",
                                        jmodelica_dir / "include/RuntimeLibrary/LazyEvaluation",
                                        jmodelica_dir / "include/RuntimeLibrary/module_include",
                                        // jmodelica_dir / "include/RuntimeLibrary/module_include",
                                        // jmodelica_dir / "include/RuntimeLibrary/zlib",
                                        jmodelica_dir / "ThirdParty/FMI/2.0",
                                        embedded_files_temp_dir / "usr/lib/llvm-10/lib/clang/10.0.0/include",
                                        embedded_files_temp_dir / "usr/include",
                                        embedded_files_temp_dir / "usr/include/linux",
                                        embedded_files_temp_dir / "usr/include/x86_64-linux-gnu",
                                        spawn::msl_path() / "Modelica/Resources/C-Sources",
                                        // The mbl_paths are a hack because our compile rule is not aware of Modelica
                                        // annoations, such as the following....
                                        //    annotation (
                                        //      IncludeDirectory="modelica://Buildings/Resources/C-Sources",
                                        // Which is used by the Modelica external C objects
                                        // It would be good to generalize this so that it works for all Modelica
                                        // libraries that may use external objects
                                        mbl_path / "Resources/src/ThermalZones/" /
                                            spawn::mbl_energyplus_version_string() / "C-Sources/",
                                        mbl_path / "Resources/C-Sources",
                                        mbl_path / "Resources/src/fmi-library/include"};

  return result;
}

std::vector<spawn_fs::path> modelicaLibs(const spawn_fs::path &jmodelica_dir,
                                         const spawn_fs::path &embedded_files_temp_dir)
{
  const auto msl = msl_path();

  return {embedded_files_temp_dir / "usr/lib/libfmilib.a",
          msl / "Modelica/Resources/Library/linux64/libzlib.a",
          msl / "Modelica/Resources/Library/linux64/libModelicaMatIO.a",
          msl / "Modelica/Resources/Library/linux64/libModelicaExternalC.a",
          msl / "Modelica/Resources/Library/linux64/libModelicaStandardTables.a",
          msl / "Modelica/Resources/Library/linux64/libModelicaIO.a",

          jmodelica_dir / "ThirdParty/Sundials/lib/libsundials_idas.a",
          jmodelica_dir / "ThirdParty/Sundials/lib/libsundials_nvecserial.a",
          jmodelica_dir / "ThirdParty/Sundials/lib/libsundials_arkode.a",
          jmodelica_dir / "ThirdParty/Sundials/lib/libsundials_ida.a",
          jmodelica_dir / "ThirdParty/Sundials/lib/libsundials_nvecopenmp.a",
          jmodelica_dir / "ThirdParty/Sundials/lib/libsundials_cvodes.a",
          jmodelica_dir / "ThirdParty/Sundials/lib/libsundials_cvode.a",
          jmodelica_dir / "ThirdParty/Sundials/lib/libsundials_kinsol.a",
          jmodelica_dir / "ThirdParty/Minpack/lib/libcminpack.a",

          jmodelica_dir / "lib/RuntimeLibrary/libjmi.a",
          jmodelica_dir / "lib/RuntimeLibrary/libblas.a",
          jmodelica_dir / "lib/RuntimeLibrary/libjmi_evaluator_util.a",
          jmodelica_dir / "lib/RuntimeLibrary/libfmi2.a",
          jmodelica_dir / "lib/RuntimeLibrary/libfmi1_me.a",
          jmodelica_dir / "lib/RuntimeLibrary/libfmi1_cs.a",
          jmodelica_dir / "lib/RuntimeLibrary/libjmi_block_solver.a",
          jmodelica_dir / "lib/RuntimeLibrary/libjmi_get_set_default.a",
          jmodelica_dir / "lib/RuntimeLibrary/libjmi_get_set_lazy.a",
          jmodelica_dir / "lib/RuntimeLibrary/liblapack.a",
          jmodelica_dir / "lib/RuntimeLibrary/libModelicaUtilities.a",

          embedded_files_temp_dir / spawn::gfortranlib_embedded_path()};
}

std::vector<spawn_fs::path> additionalSource()
{
  const auto mbl_path = mblPathFromEnv();
  std::vector<spawn_fs::path> result;

  for (const auto &entry : spawn_fs::directory_iterator(mbl_path / "Resources/src/ThermalZones" /
                                                        spawn::mbl_energyplus_version_string() / "C-Sources/")) {
    if (entry.path().extension() == ".c") {
      result.push_back(entry.path());
    }
  }

  return result;
}

int compileMO(const std::string &moInput,
              const spawn_fs::path &outputDir,
              const std::vector<std::string> &modelicaPaths,
              const FMUType &fmuType,
              const ModelicaCompilerType &moType)
{
  try {
    // Only primitive data types can be passed from C++ to Java
    // so JSON is serialized and converted to a raw character array
    json j;
    j["model"] = moInput;
    j["outputDir"] = outputDir.generic_string();
    j["mslDir"] = msl_path().generic_string();
    j["fmuType"] = toString(fmuType);
    j["modelicaPaths"] = modelicaPaths;

    std::string params = j.dump();
    std::vector<char> cparams(params.begin(), params.end());
    cparams.resize(params.size() + 1, 0); // make sure we have a null terminator

    graal_isolate_t *isolate = nullptr;
    graal_isolatethread_t *thread = nullptr;

    // The original idea was to allow switching between jmodelica and optimica,
    // but there are challenges related to graal that might make this difficult or impossible.
    if (moType == ModelicaCompilerType::Optimica) {
      optimica_create_isolate(nullptr, &isolate, &thread);
      optimica_compile(thread, cparams.data());
    } else {
      jmodelica_create_isolate(nullptr, &isolate, &thread);
      jmodelica_compile(thread, cparams.data());
    }

    return 0;
  } catch (...) {
    return 1;
  }
}

int compileC(const spawn_fs::path &sources_dir,
             const spawn_fs::path &model_lib_path,
             const spawn_fs::path &jmodelica_dir,
             const spawn_fs::path &embedded_files_temp_dir)
{
  std::vector<spawn_fs::path> sourcefiles;

  if (!spawn_fs::is_directory(sources_dir)) {
    return 1;
  }

  for (const auto &p : spawn_fs::directory_iterator(sources_dir)) {
    sourcefiles.push_back(p.path());
  }

  // Exclude files that are not C source files
  sourcefiles.erase(
      std::remove_if(begin(sourcefiles), end(sourcefiles), [](const auto &p) { return (p.extension() != ".c"); }),
      end(sourcefiles));

  // Exclude ModelicaStrings_skipWhiteSpace.c
  // These symbols seem to be already defined within the runtime libraries
  // Excluding to avoid duplicate symbol error from compiler
  sourcefiles.erase(
      std::remove_if(begin(sourcefiles),
                     end(sourcefiles),
                     [](const auto &p) { return (p.filename().string() == "ModelicaStrings_skipWhiteSpace.c"); }),
      end(sourcefiles));

  sourcefiles.erase(
      std::remove_if(begin(sourcefiles),
                     end(sourcefiles),
                     [](const auto &p) { return (p.filename().string() == "ModelicaStrings_compare.c"); }),
      end(sourcefiles));

  const auto source_to_add = additionalSource();
  sourcefiles.insert(sourcefiles.end(), source_to_add.begin(), source_to_add.end());

  // src/JModelica/Makefiles/MakeFile
  // Setup link libs and include paths
  // This could be improved, by referring to the MakeFile that JModelica normally uses
  // Refer to JModelica/Makefiles/MakeFile
  //
  // Also need a way to handle external C code that may be referred to and contained within
  // some Modelica models

  // Libs to link
  const auto runtime_libs = modelicaLibs(jmodelica_dir, embedded_files_temp_dir);

  // include paths
  auto include_paths = includePaths(jmodelica_dir, embedded_files_temp_dir);

  const std::vector<std::string> flags{
      "-Wno-enum-conversion", "-Wno-incompatible-pointer-types-discards-qualifiers", "-Wno-logical-not-parentheses"};
  spawn::Compiler compiler(include_paths, flags);

  std::for_each(begin(sourcefiles), end(sourcefiles), [&](const auto &path) { compiler.compile_and_link(path); });

  spawn_fs::create_directories(model_lib_path.parent_path());
  compiler.write_shared_object_file(model_lib_path, embedded_files_temp_dir, runtime_libs);

  return 0;
}

void makeModelicaExternalFunction(const std::vector<std::string> &parameters)
{
  for (const auto &param : parameters) {
    spdlog::trace("makeModelicalExternalFunction parameter {}", param);
  }

  if (parameters.size() < 3) {
    spdlog::error("unable to determine build target, aborting");
    return;
  }

  if (parameters[3] == "ceval") {
    spdlog::info("ceval: compiling executable from .c files");
  } else {
    spdlog::error("Unknown build target: '{}' aborting", parameters[5]);
    return;
  }

  std::map<std::string, std::string> arguments;

  for (const auto &param : parameters) {
    const auto equals_pos = param.find('=');

    if (equals_pos != std::string::npos) {
      arguments[param.substr(0, equals_pos)] = param.substr(equals_pos + 1);
    }
  }

  for (const auto &[lhs, rhs] : arguments) {
    spdlog::trace("Parsed make modelica arg '{}'='{}'", lhs, rhs);
  }

  spawn_fs::path fileToCompile = spawn_fs::path{"sources"} / arguments["FILE_NAME"];
  fileToCompile += ".c";

  const auto jmodelica_dir = spawn_fs::path{arguments["JMODELICA_HOME"]};
  const auto embedded_files_temp_dir = jmodelica_dir.parent_path();
  const auto include_paths = includePaths(jmodelica_dir, embedded_files_temp_dir);
  const auto runtime_libs = modelicaLibs(jmodelica_dir, embedded_files_temp_dir);

  std::cout << "jmodelica_dir: " << jmodelica_dir << std::endl;

  const std::vector<std::string> flags{};
  spawn::Compiler compiler(include_paths, flags);

  compiler.compile_and_link(fileToCompile);
  spawn_fs::create_directories(spawn_fs::path{"binaries"});

  // we'll name it .so regardless of platform because it's only for our use anyhow
  const auto launcherFileName = spawn_fs::path{"binaries"} / "spawn_exe_launcher";
  const auto exeFileName = spawn_fs::path{"binaries"} / arguments["FILE_NAME"];
  const auto soFileName = [&]() {
    auto result = exeFileName;
    result.replace_extension(exeFileName.extension().string() + ".so");
    return result;
  }();

  compiler.write_shared_object_file(soFileName, embedded_files_temp_dir, runtime_libs);
  spdlog::info("Modelical shared object output: {} exists {}", soFileName.string(), spawn_fs::exists(soFileName));

  // To support Windows this needs to be configured for extension
  spawnmodelica::embedded_files::extractFile(":/spawn_exe_launcher", "binaries");
  spawn_fs::rename(launcherFileName, exeFileName);

  spawn_fs::permissions(exeFileName, spawn_fs::perms::owner_exec);
}

void extractEmbeddedCompilerFiles(const spawn_fs::path &dir, const ModelicaCompilerType &)
{
  spawn_fs::create_directories(dir);

  // TODO: This includes both jmodelica and optimica,
  // it would be better to select only the compiler files we need
  for (const auto &file : spawnmodelica::embedded_files::fileNames()) {
    spawnmodelica::embedded_files::extractFile(file, dir.native());
  }

  // The embedded filesystem does not preserve permission so this is an ugly but important step
  // To support Windows this needs to be configured for extension
  const spawn_fs::path licenseExecutable = dir / "Optimica/lib/LicensingEncryption/linux/leif_mlle";
  spawn_fs::permissions(licenseExecutable, spawn_fs::perms::owner_exec | spawn_fs::perms::owner_read);

  // To support Windows this needs to be configured for extension
  const spawn_fs::path jmiEvaluatorExecutable = dir / "Optimica/bin/jmi_evaluator";
  spawn_fs::permissions(jmiEvaluatorExecutable, spawn_fs::perms::owner_exec | spawn_fs::perms::owner_read);
}

void chmodFilesInPath(const spawn_fs::path &path, const spawn_fs::perms perm)
{
  for (const auto &entry : spawn_fs::directory_iterator{path}) {
    spawn_fs::permissions(entry, perm);
  }
}

int modelicaToFMU(const std::string &moinput,
                  std::vector<std::string> modelicaPaths,
                  const FMUType &fmuType,
                  const ModelicaCompilerType &moType)
{
  // output_dir_name is moinput with "." replaced by "_"
  std::string output_dir_name;
  std::transform(begin(moinput), end(moinput), std::back_inserter(output_dir_name), [](const auto &c) {
    if (c == '.') {
      return '_';
    } else {
      return c;
    }
  });

  const auto output_dir = spawn_fs::current_path() / output_dir_name;
  const auto fmu_path = output_dir.parent_path() / (output_dir_name + ".fmu");
  const auto sources_dir = output_dir / "sources";
  const auto binary_dir = output_dir / "binaries" / fmi_platform();
  const auto model_description_path = output_dir / "modelDescription.xml";

  if (!output_dir_name.empty()) {
    spawn_fs::remove_all(output_dir);
  }
  if (spawn_fs::exists(fmu_path)) {
    spawn_fs::remove_all(fmu_path);
  }

  // tmp is where we extract embedded files
  const auto &temp_dir = output_dir / "tmp";
  extractEmbeddedCompilerFiles(temp_dir, moType);

  auto jmodelica_dir = temp_dir / "JModelica";
  if (moType == ModelicaCompilerType::Optimica) {
    jmodelica_dir = temp_dir / "Optimica";
  }

  modelicaPaths.push_back(spawn::msl_path());
  const auto mbl_path = mblPathInPaths(modelicaPaths);
  if (mbl_path.empty()) {
    modelicaPaths.push_back(mbl_home_dir().generic_string());
  }

  setenv("JMODELICA_HOME", jmodelica_dir.generic_string().c_str(), 1);
  setenv("MODELICAPATH", pathVectorToPath(modelicaPaths).c_str(), 1);

  int result = compileMO(moinput, output_dir.native(), modelicaPaths, fmuType, moType);

  if (result == 0) {
    spdlog::info("Compile C Code");
    spawn::fmu::ModelDescription md(model_description_path);
    spawn_fs::path model_lib_path = binary_dir / (md.modelIdentifier() + ".so");
    result = compileC(sources_dir, model_lib_path, jmodelica_dir, temp_dir);
  }

  spawn_fs::remove_all(sources_dir);

  if (result == 0) {
    // Bundle GFortran with the FMU, since it is a required dependency, but not provided out of the box on most systems
    spawn_fs::copy(temp_dir / spawn::gfortranlib_embedded_path(), binary_dir);

    // Remove temp_dir which is where the embedded filesystem was extracted
    spawn_fs::remove_all(temp_dir);
    // Update permissions to work in a variety of scenarios, including from within containers
    const auto perm = spawn_fs::perms::owner_all | spawn_fs::perms::group_read | spawn_fs::perms::group_exec |
                      spawn_fs::perms::others_read | spawn_fs::perms::others_exec;
    chmodFilesInPath(binary_dir, perm);
    spdlog::info("Compress FMU");
    zip_directory(output_dir.string(), fmu_path.string(), false);
    spawn_fs::remove_all(output_dir);
    spdlog::info("Model Compiled");
  } else {
    spdlog::info("Model Failed to Compile");
    spdlog::info("Input files can be inspected here: {}", output_dir.string());
  }

  return result;
}

} // namespace spawn
