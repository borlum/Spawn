# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.8.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.8.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/kbenne/Development/jmodelica/trunk/ThirdParty/Sundials/sundials-2.7.0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/kbenne/Development/jmodelica/trunk/build

# Include any dependencies generated for this target.
include examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/depend.make

# Include the progress variables for this target.
include examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/progress.make

# Include the compile flags for this target's objects.
include examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/flags.make

examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.o: examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/flags.make
examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.o: /Users/kbenne/Development/jmodelica/trunk/ThirdParty/Sundials/sundials-2.7.0/examples/cvode/serial/cvDiurnal_kry.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kbenne/Development/jmodelica/trunk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.o"
	cd /Users/kbenne/Development/jmodelica/trunk/build/examples/cvode/serial && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.o   -c /Users/kbenne/Development/jmodelica/trunk/ThirdParty/Sundials/sundials-2.7.0/examples/cvode/serial/cvDiurnal_kry.c

examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.i"
	cd /Users/kbenne/Development/jmodelica/trunk/build/examples/cvode/serial && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/kbenne/Development/jmodelica/trunk/ThirdParty/Sundials/sundials-2.7.0/examples/cvode/serial/cvDiurnal_kry.c > CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.i

examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.s"
	cd /Users/kbenne/Development/jmodelica/trunk/build/examples/cvode/serial && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/kbenne/Development/jmodelica/trunk/ThirdParty/Sundials/sundials-2.7.0/examples/cvode/serial/cvDiurnal_kry.c -o CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.s

examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.o.requires:

.PHONY : examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.o.requires

examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.o.provides: examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.o.requires
	$(MAKE) -f examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/build.make examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.o.provides.build
.PHONY : examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.o.provides

examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.o.provides.build: examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.o


# Object files for target cvDiurnal_kry
cvDiurnal_kry_OBJECTS = \
"CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.o"

# External object files for target cvDiurnal_kry
cvDiurnal_kry_EXTERNAL_OBJECTS =

examples/cvode/serial/cvDiurnal_kry: examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.o
examples/cvode/serial/cvDiurnal_kry: examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/build.make
examples/cvode/serial/cvDiurnal_kry: src/cvode/libsundials_cvode.2.9.0.dylib
examples/cvode/serial/cvDiurnal_kry: src/nvec_ser/libsundials_nvecserial.2.7.0.dylib
examples/cvode/serial/cvDiurnal_kry: examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/kbenne/Development/jmodelica/trunk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable cvDiurnal_kry"
	cd /Users/kbenne/Development/jmodelica/trunk/build/examples/cvode/serial && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cvDiurnal_kry.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/build: examples/cvode/serial/cvDiurnal_kry

.PHONY : examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/build

examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/requires: examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/cvDiurnal_kry.c.o.requires

.PHONY : examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/requires

examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/clean:
	cd /Users/kbenne/Development/jmodelica/trunk/build/examples/cvode/serial && $(CMAKE_COMMAND) -P CMakeFiles/cvDiurnal_kry.dir/cmake_clean.cmake
.PHONY : examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/clean

examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/depend:
	cd /Users/kbenne/Development/jmodelica/trunk/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kbenne/Development/jmodelica/trunk/ThirdParty/Sundials/sundials-2.7.0 /Users/kbenne/Development/jmodelica/trunk/ThirdParty/Sundials/sundials-2.7.0/examples/cvode/serial /Users/kbenne/Development/jmodelica/trunk/build /Users/kbenne/Development/jmodelica/trunk/build/examples/cvode/serial /Users/kbenne/Development/jmodelica/trunk/build/examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/cvode/serial/CMakeFiles/cvDiurnal_kry.dir/depend

