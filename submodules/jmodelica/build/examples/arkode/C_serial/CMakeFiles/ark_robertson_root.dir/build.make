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
include examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/depend.make

# Include the progress variables for this target.
include examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/progress.make

# Include the compile flags for this target's objects.
include examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/flags.make

examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.o: examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/flags.make
examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.o: /Users/kbenne/Development/jmodelica/trunk/ThirdParty/Sundials/sundials-2.7.0/examples/arkode/C_serial/ark_robertson_root.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kbenne/Development/jmodelica/trunk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.o"
	cd /Users/kbenne/Development/jmodelica/trunk/build/examples/arkode/C_serial && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.o   -c /Users/kbenne/Development/jmodelica/trunk/ThirdParty/Sundials/sundials-2.7.0/examples/arkode/C_serial/ark_robertson_root.c

examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.i"
	cd /Users/kbenne/Development/jmodelica/trunk/build/examples/arkode/C_serial && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/kbenne/Development/jmodelica/trunk/ThirdParty/Sundials/sundials-2.7.0/examples/arkode/C_serial/ark_robertson_root.c > CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.i

examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.s"
	cd /Users/kbenne/Development/jmodelica/trunk/build/examples/arkode/C_serial && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/kbenne/Development/jmodelica/trunk/ThirdParty/Sundials/sundials-2.7.0/examples/arkode/C_serial/ark_robertson_root.c -o CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.s

examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.o.requires:

.PHONY : examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.o.requires

examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.o.provides: examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.o.requires
	$(MAKE) -f examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/build.make examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.o.provides.build
.PHONY : examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.o.provides

examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.o.provides.build: examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.o


# Object files for target ark_robertson_root
ark_robertson_root_OBJECTS = \
"CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.o"

# External object files for target ark_robertson_root
ark_robertson_root_EXTERNAL_OBJECTS =

examples/arkode/C_serial/ark_robertson_root: examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.o
examples/arkode/C_serial/ark_robertson_root: examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/build.make
examples/arkode/C_serial/ark_robertson_root: src/arkode/libsundials_arkode.1.1.0.dylib
examples/arkode/C_serial/ark_robertson_root: src/nvec_ser/libsundials_nvecserial.2.7.0.dylib
examples/arkode/C_serial/ark_robertson_root: examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/kbenne/Development/jmodelica/trunk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ark_robertson_root"
	cd /Users/kbenne/Development/jmodelica/trunk/build/examples/arkode/C_serial && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ark_robertson_root.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/build: examples/arkode/C_serial/ark_robertson_root

.PHONY : examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/build

examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/requires: examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/ark_robertson_root.c.o.requires

.PHONY : examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/requires

examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/clean:
	cd /Users/kbenne/Development/jmodelica/trunk/build/examples/arkode/C_serial && $(CMAKE_COMMAND) -P CMakeFiles/ark_robertson_root.dir/cmake_clean.cmake
.PHONY : examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/clean

examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/depend:
	cd /Users/kbenne/Development/jmodelica/trunk/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kbenne/Development/jmodelica/trunk/ThirdParty/Sundials/sundials-2.7.0 /Users/kbenne/Development/jmodelica/trunk/ThirdParty/Sundials/sundials-2.7.0/examples/arkode/C_serial /Users/kbenne/Development/jmodelica/trunk/build /Users/kbenne/Development/jmodelica/trunk/build/examples/arkode/C_serial /Users/kbenne/Development/jmodelica/trunk/build/examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/arkode/C_serial/CMakeFiles/ark_robertson_root.dir/depend

