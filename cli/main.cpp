#include "../util/platform.hpp"
#include "../lib/fmugenerator.hpp"
#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>
#include <cstdio>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <boost/filesystem.hpp>
#include <config.hxx>
#include <boost/algorithm/string.hpp>
#include <stdlib.h>
#include "../util/fmi_paths.hpp"

#if defined _WIN32
#include <windows.h>
#else
#include <stdio.h>
#include <dlfcn.h>
#endif

#if defined ENABLE_MODELICA_COMPILER
#include "compilerchain.hpp"
#endif

using json = nlohmann::json;

boost::filesystem::path exedir() {
  #if _WIN32
    TCHAR szPath[MAX_PATH];
    GetModuleFileName(nullptr, szPath, MAX_PATH);
    return boost::filesystem::path(szPath).parent_path();
  #else
    Dl_info info;
    dladdr("main", &info);
    return boost::filesystem::path(info.dli_fname).parent_path();
  #endif
}

bool isInstalled() {
  return exedir().stem() == "bin";
}

boost::filesystem::path iddInstallPath() {
  constexpr auto & iddfilename = "Energy+.idd";
  // Configuration in install tree
  auto iddInputPath = exedir() / "../etc" / iddfilename;

  // Configuration in a developer tree
  if (! boost::filesystem::exists(iddInputPath)) {
    iddInputPath = exedir() / iddfilename;
  }

  return iddInputPath;
}

boost::filesystem::path epfmiInstallPath() {
  const auto candidate = exedir() / ("../lib/" + spawn::epfmiName());
  if (boost::filesystem::exists(candidate)) {
    return candidate;
  } else {
    return exedir() / spawn::epfmiName();
  }
}

boost::filesystem::path jmodelicaHome() {
  if (isInstalled()) {
    return exedir() / "../JModelica/";
  } else {
    boost::filesystem::path binary_dir(spawn::BINARY_DIR);
    return binary_dir / "JModelica/";
  }
}

boost::filesystem::path mblPath() {
  if (isInstalled()) {
    return exedir() / "../modelica-buildings/Buildings/";
  } else {
    boost::filesystem::path source_dir(spawn::SOURCE_DIR);
    return source_dir / "submodules/modelica-buildings/Buildings/";
  }
}

void handle_eptr(std::exception_ptr eptr) {
  try {
    if (eptr) {
      std::rethrow_exception(eptr);
    }
  } catch(const std::exception& e) {
    std::cout << "Spawn encountered an error: \n\"" << e.what() << "\"\n";
  }
}

int main(int argc, const char *argv[]) {
  CLI::App app{"Spawn of EnergyPlus"};

  std::string jsoninput = "spawn.json";
  auto createOption =
      app.add_option("-c,--create", jsoninput,
                     "Create a standalone FMU based on json input", true);

  std::string outputpath;
  auto outputPathOption =
      app.add_option("--output-path", outputpath,
                     "Full path including filename and extension where the fmu should be placed. Intermediate directories will be created if necessary", true);
  outputPathOption->needs(createOption);

  std::string outputdir;
  auto outputDirOption =
      app.add_option("--output-dir", outputdir,
                     "Directory where the fmu should be placed. This path will be created if necessary", true);
  outputDirOption->needs(createOption);

  outputDirOption->excludes(outputPathOption);
  outputPathOption->excludes(outputDirOption);

  bool nozip = false;
  auto zipOption = app.add_flag("--no-zip", nozip, "Stage FMU files on disk without creating a zip archive");
  zipOption->needs(createOption);

  bool nocompress = false;
  auto compressOption = app.add_flag("--no-compress", nocompress, "Skip compressing the contents of the fmu zip archive. An uncompressed zip archive will be created instead.");
  compressOption->needs(createOption);

  auto versionOption =
    app.add_flag("-v,--version", "Print version info and exit");

#if defined ENABLE_MODELICA_COMPILER
  std::string moinput = "";
  auto compileOption =
      app.add_option("--compile", moinput,
                     "Compile Modelica model to FMU format", true);
  auto cCompiler = app.add_subcommand("clang", "Pass all remaining arguments to the internal clang compiler (not yet implemented)");
  auto makeExternal = app.add_subcommand("-f", "compile a Modelica external function, acting like 'make'");
  makeExternal->prefix_command();
#endif




  CLI11_PARSE(app, argc, argv);

  std::exception_ptr eptr;
  try {
    if (*createOption) {
      spawn::energyplusToFMU(jsoninput, nozip, nocompress, outputpath, outputdir, iddInstallPath(), epfmiInstallPath());
#if defined ENABLE_MODELICA_COMPILER
    } else if (*compileOption) {
      spawn::modelicaToFMU(moinput, mblPath(), jmodelicaHome());
    } else if (makeExternal->parsed()) {
      spawn::makeModelicaExternalFunction({argv, std::next(argv, argc)});
#endif
    } else if (*versionOption) {
      std::cout << "Spawn-" << spawn::VERSION_STRING << std::endl;
    }
  } catch(...) {
    eptr = std::current_exception();
  }

  if (eptr) {
    handle_eptr(eptr);
    return 1;
  } else {
    return 0;
  }
}

