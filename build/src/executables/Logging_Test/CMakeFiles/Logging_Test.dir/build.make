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
include src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/depend.make

# Include the progress variables for this target.
include src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/progress.make

# Include the compile flags for this target's objects.
include src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/flags.make

src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/main.cpp.o: src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/flags.make
src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/main.cpp.o: ../src/executables/Logging_Test/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/main.cpp.o"
	cd /home/maik/projects/lisa_geko/GeKo/build/src/executables/Logging_Test && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Logging_Test.dir/main.cpp.o -c /home/maik/projects/lisa_geko/GeKo/src/executables/Logging_Test/main.cpp

src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Logging_Test.dir/main.cpp.i"
	cd /home/maik/projects/lisa_geko/GeKo/build/src/executables/Logging_Test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/maik/projects/lisa_geko/GeKo/src/executables/Logging_Test/main.cpp > CMakeFiles/Logging_Test.dir/main.cpp.i

src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Logging_Test.dir/main.cpp.s"
	cd /home/maik/projects/lisa_geko/GeKo/build/src/executables/Logging_Test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/maik/projects/lisa_geko/GeKo/src/executables/Logging_Test/main.cpp -o CMakeFiles/Logging_Test.dir/main.cpp.s

src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/main.cpp.o.requires:
.PHONY : src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/main.cpp.o.requires

src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/main.cpp.o.provides: src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/main.cpp.o.requires
	$(MAKE) -f src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/build.make src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/main.cpp.o.provides.build
.PHONY : src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/main.cpp.o.provides

src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/main.cpp.o.provides.build: src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/main.cpp.o

# Object files for target Logging_Test
Logging_Test_OBJECTS = \
"CMakeFiles/Logging_Test.dir/main.cpp.o"

# External object files for target Logging_Test
Logging_Test_EXTERNAL_OBJECTS =

bin/Logging_Test: src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/main.cpp.o
bin/Logging_Test: src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/build.make
bin/Logging_Test: /usr/lib/x86_64-linux-gnu/libSM.so
bin/Logging_Test: /usr/lib/x86_64-linux-gnu/libICE.so
bin/Logging_Test: /usr/lib/x86_64-linux-gnu/libX11.so
bin/Logging_Test: /usr/lib/x86_64-linux-gnu/libXext.so
bin/Logging_Test: lib/libGeKo_Gameplay.a
bin/Logging_Test: lib/libGeKo_Physics.a
bin/Logging_Test: lib/libLogging.a
bin/Logging_Test: lib/libGeKo_Sound.a
bin/Logging_Test: lib/libGeKo_Graphics.a
bin/Logging_Test: lib/libGeKo_Debugger.a
bin/Logging_Test: dependencies/glfw/src/glfw-build/src/libglfw3.a
bin/Logging_Test: dependencies/glew/src/glew-build/lib/libglew.a
bin/Logging_Test: /usr/lib/libassimp.so
bin/Logging_Test: dependencies/OpenAL/src/OpenAL-build/libopenal.so
bin/Logging_Test: dependencies/TinyXML/src/TinyXML-build/libtinyxml2.so
bin/Logging_Test: lib/libGeKo_Gameplay.a
bin/Logging_Test: lib/libGeKo_Physics.a
bin/Logging_Test: lib/libLogging.a
bin/Logging_Test: lib/libGeKo_Sound.a
bin/Logging_Test: lib/libGeKo_Graphics.a
bin/Logging_Test: lib/libGeKo_Debugger.a
bin/Logging_Test: /usr/lib/x86_64-linux-gnu/libSM.so
bin/Logging_Test: /usr/lib/x86_64-linux-gnu/libICE.so
bin/Logging_Test: /usr/lib/x86_64-linux-gnu/libX11.so
bin/Logging_Test: /usr/lib/x86_64-linux-gnu/libXext.so
bin/Logging_Test: dependencies/glfw/src/glfw-build/src/libglfw3.a
bin/Logging_Test: dependencies/glew/src/glew-build/lib/libglew.a
bin/Logging_Test: /usr/lib/libassimp.so
bin/Logging_Test: dependencies/OpenAL/src/OpenAL-build/libopenal.so
bin/Logging_Test: dependencies/TinyXML/src/TinyXML-build/libtinyxml2.so
bin/Logging_Test: src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../../bin/Logging_Test"
	cd /home/maik/projects/lisa_geko/GeKo/build/src/executables/Logging_Test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Logging_Test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/build: bin/Logging_Test
.PHONY : src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/build

src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/requires: src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/main.cpp.o.requires
.PHONY : src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/requires

src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/clean:
	cd /home/maik/projects/lisa_geko/GeKo/build/src/executables/Logging_Test && $(CMAKE_COMMAND) -P CMakeFiles/Logging_Test.dir/cmake_clean.cmake
.PHONY : src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/clean

src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/depend:
	cd /home/maik/projects/lisa_geko/GeKo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maik/projects/lisa_geko/GeKo /home/maik/projects/lisa_geko/GeKo/src/executables/Logging_Test /home/maik/projects/lisa_geko/GeKo/build /home/maik/projects/lisa_geko/GeKo/build/src/executables/Logging_Test /home/maik/projects/lisa_geko/GeKo/build/src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/executables/Logging_Test/CMakeFiles/Logging_Test.dir/depend

