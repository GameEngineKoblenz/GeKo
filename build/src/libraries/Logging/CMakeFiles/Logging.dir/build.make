# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/maik/projects/lisa_geko/GeKo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/maik/projects/lisa_geko/GeKo/build

# Include any dependencies generated for this target.
include src/libraries/Logging/CMakeFiles/Logging.dir/depend.make

# Include the progress variables for this target.
include src/libraries/Logging/CMakeFiles/Logging.dir/progress.make

# Include the compile flags for this target's objects.
include src/libraries/Logging/CMakeFiles/Logging.dir/flags.make

src/libraries/Logging/CMakeFiles/Logging.dir/Utility.cpp.o: src/libraries/Logging/CMakeFiles/Logging.dir/flags.make
src/libraries/Logging/CMakeFiles/Logging.dir/Utility.cpp.o: ../src/libraries/Logging/Utility.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/libraries/Logging/CMakeFiles/Logging.dir/Utility.cpp.o"
	cd /home/maik/projects/lisa_geko/GeKo/build/src/libraries/Logging && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Logging.dir/Utility.cpp.o -c /home/maik/projects/lisa_geko/GeKo/src/libraries/Logging/Utility.cpp

src/libraries/Logging/CMakeFiles/Logging.dir/Utility.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Logging.dir/Utility.cpp.i"
	cd /home/maik/projects/lisa_geko/GeKo/build/src/libraries/Logging && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/maik/projects/lisa_geko/GeKo/src/libraries/Logging/Utility.cpp > CMakeFiles/Logging.dir/Utility.cpp.i

src/libraries/Logging/CMakeFiles/Logging.dir/Utility.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Logging.dir/Utility.cpp.s"
	cd /home/maik/projects/lisa_geko/GeKo/build/src/libraries/Logging && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/maik/projects/lisa_geko/GeKo/src/libraries/Logging/Utility.cpp -o CMakeFiles/Logging.dir/Utility.cpp.s

src/libraries/Logging/CMakeFiles/Logging.dir/Utility.cpp.o.requires:
.PHONY : src/libraries/Logging/CMakeFiles/Logging.dir/Utility.cpp.o.requires

src/libraries/Logging/CMakeFiles/Logging.dir/Utility.cpp.o.provides: src/libraries/Logging/CMakeFiles/Logging.dir/Utility.cpp.o.requires
	$(MAKE) -f src/libraries/Logging/CMakeFiles/Logging.dir/build.make src/libraries/Logging/CMakeFiles/Logging.dir/Utility.cpp.o.provides.build
.PHONY : src/libraries/Logging/CMakeFiles/Logging.dir/Utility.cpp.o.provides

src/libraries/Logging/CMakeFiles/Logging.dir/Utility.cpp.o.provides.build: src/libraries/Logging/CMakeFiles/Logging.dir/Utility.cpp.o

src/libraries/Logging/CMakeFiles/Logging.dir/Log.cpp.o: src/libraries/Logging/CMakeFiles/Logging.dir/flags.make
src/libraries/Logging/CMakeFiles/Logging.dir/Log.cpp.o: ../src/libraries/Logging/Log.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/libraries/Logging/CMakeFiles/Logging.dir/Log.cpp.o"
	cd /home/maik/projects/lisa_geko/GeKo/build/src/libraries/Logging && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Logging.dir/Log.cpp.o -c /home/maik/projects/lisa_geko/GeKo/src/libraries/Logging/Log.cpp

src/libraries/Logging/CMakeFiles/Logging.dir/Log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Logging.dir/Log.cpp.i"
	cd /home/maik/projects/lisa_geko/GeKo/build/src/libraries/Logging && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/maik/projects/lisa_geko/GeKo/src/libraries/Logging/Log.cpp > CMakeFiles/Logging.dir/Log.cpp.i

src/libraries/Logging/CMakeFiles/Logging.dir/Log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Logging.dir/Log.cpp.s"
	cd /home/maik/projects/lisa_geko/GeKo/build/src/libraries/Logging && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/maik/projects/lisa_geko/GeKo/src/libraries/Logging/Log.cpp -o CMakeFiles/Logging.dir/Log.cpp.s

src/libraries/Logging/CMakeFiles/Logging.dir/Log.cpp.o.requires:
.PHONY : src/libraries/Logging/CMakeFiles/Logging.dir/Log.cpp.o.requires

src/libraries/Logging/CMakeFiles/Logging.dir/Log.cpp.o.provides: src/libraries/Logging/CMakeFiles/Logging.dir/Log.cpp.o.requires
	$(MAKE) -f src/libraries/Logging/CMakeFiles/Logging.dir/build.make src/libraries/Logging/CMakeFiles/Logging.dir/Log.cpp.o.provides.build
.PHONY : src/libraries/Logging/CMakeFiles/Logging.dir/Log.cpp.o.provides

src/libraries/Logging/CMakeFiles/Logging.dir/Log.cpp.o.provides.build: src/libraries/Logging/CMakeFiles/Logging.dir/Log.cpp.o

# Object files for target Logging
Logging_OBJECTS = \
"CMakeFiles/Logging.dir/Utility.cpp.o" \
"CMakeFiles/Logging.dir/Log.cpp.o"

# External object files for target Logging
Logging_EXTERNAL_OBJECTS =

lib/libLogging.a: src/libraries/Logging/CMakeFiles/Logging.dir/Utility.cpp.o
lib/libLogging.a: src/libraries/Logging/CMakeFiles/Logging.dir/Log.cpp.o
lib/libLogging.a: src/libraries/Logging/CMakeFiles/Logging.dir/build.make
lib/libLogging.a: src/libraries/Logging/CMakeFiles/Logging.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library ../../../lib/libLogging.a"
	cd /home/maik/projects/lisa_geko/GeKo/build/src/libraries/Logging && $(CMAKE_COMMAND) -P CMakeFiles/Logging.dir/cmake_clean_target.cmake
	cd /home/maik/projects/lisa_geko/GeKo/build/src/libraries/Logging && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Logging.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/libraries/Logging/CMakeFiles/Logging.dir/build: lib/libLogging.a
.PHONY : src/libraries/Logging/CMakeFiles/Logging.dir/build

src/libraries/Logging/CMakeFiles/Logging.dir/requires: src/libraries/Logging/CMakeFiles/Logging.dir/Utility.cpp.o.requires
src/libraries/Logging/CMakeFiles/Logging.dir/requires: src/libraries/Logging/CMakeFiles/Logging.dir/Log.cpp.o.requires
.PHONY : src/libraries/Logging/CMakeFiles/Logging.dir/requires

src/libraries/Logging/CMakeFiles/Logging.dir/clean:
	cd /home/maik/projects/lisa_geko/GeKo/build/src/libraries/Logging && $(CMAKE_COMMAND) -P CMakeFiles/Logging.dir/cmake_clean.cmake
.PHONY : src/libraries/Logging/CMakeFiles/Logging.dir/clean

src/libraries/Logging/CMakeFiles/Logging.dir/depend:
	cd /home/maik/projects/lisa_geko/GeKo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maik/projects/lisa_geko/GeKo /home/maik/projects/lisa_geko/GeKo/src/libraries/Logging /home/maik/projects/lisa_geko/GeKo/build /home/maik/projects/lisa_geko/GeKo/build/src/libraries/Logging /home/maik/projects/lisa_geko/GeKo/build/src/libraries/Logging/CMakeFiles/Logging.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/libraries/Logging/CMakeFiles/Logging.dir/depend

