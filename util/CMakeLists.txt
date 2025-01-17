if(UNIX)
  add_definitions("-fPIC")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

configure_file(config.cpp.in config.cxx)

add_library(
  spawn_utils
  compare.hpp
  compare.cpp
  filesystem.hpp
  fmi_paths.hpp
  temp_directory.cpp
  temp_directory.hpp
  unzipped_file.cpp
  unzipped_file.hpp
  dynamiclibrary.hpp
  dynamiclibrary_windows.hpp
  dynamiclibrary_posix.hpp
  unique_id.hpp
  unique_id.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/config.cxx
)

target_link_libraries(
  spawn_utils
  PRIVATE CONAN_PKG::libzip CONAN_PKG::fmt CONAN_PKG::spdlog compile_options)

include(CheckLinkerFlag)
check_linker_flag(CXX -lstdc++fs HAS_FS_FLAG)
if(HAS_FS_FLAG)
  target_link_libraries(spawn_utils PUBLIC stdc++fs)
endif()
