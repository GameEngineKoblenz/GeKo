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
include src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/depend.make

# Include the progress variables for this target.
include src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/progress.make

# Include the compile flags for this target's objects.
include src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/flags.make

src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.o: src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/flags.make
src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.o: ../src/executables/Ex_GraphicShowcase/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.o"
	cd /home/maik/projects/lisa_geko/GeKo/build/src/executables/Ex_GraphicShowcase && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.o -c /home/maik/projects/lisa_geko/GeKo/src/executables/Ex_GraphicShowcase/main.cpp

src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.i"
	cd /home/maik/projects/lisa_geko/GeKo/build/src/executables/Ex_GraphicShowcase && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/maik/projects/lisa_geko/GeKo/src/executables/Ex_GraphicShowcase/main.cpp > CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.i

src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.s"
	cd /home/maik/projects/lisa_geko/GeKo/build/src/executables/Ex_GraphicShowcase && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/maik/projects/lisa_geko/GeKo/src/executables/Ex_GraphicShowcase/main.cpp -o CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.s

src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.o.requires:
.PHONY : src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.o.requires

src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.o.provides: src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.o.requires
	$(MAKE) -f src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/build.make src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.o.provides.build
.PHONY : src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.o.provides

src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.o.provides.build: src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.o

# Object files for target Ex_GraphicShowcase
Ex_GraphicShowcase_OBJECTS = \
"CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.o"

# External object files for target Ex_GraphicShowcase
Ex_GraphicShowcase_EXTERNAL_OBJECTS =

bin/Ex_GraphicShowcase: src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.o
bin/Ex_GraphicShowcase: src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/build.make
bin/Ex_GraphicShowcase: /usr/lib/x86_64-linux-gnu/libSM.so
bin/Ex_GraphicShowcase: /usr/lib/x86_64-linux-gnu/libICE.so
bin/Ex_GraphicShowcase: /usr/lib/x86_64-linux-gnu/libX11.so
bin/Ex_GraphicShowcase: /usr/lib/x86_64-linux-gnu/libXext.so
bin/Ex_GraphicShowcase: lib/libGeKo_Gameplay.a
bin/Ex_GraphicShowcase: lib/libGeKo_Physics.a
bin/Ex_GraphicShowcase: lib/libLogging.a
bin/Ex_GraphicShowcase: lib/libGeKo_Sound.a
bin/Ex_GraphicShowcase: lib/libGeKo_Graphics.a
bin/Ex_GraphicShowcase: lib/libGeKo_Debugger.a
bin/Ex_GraphicShowcase: dependencies/glfw/src/glfw-build/src/libglfw3.a
bin/Ex_GraphicShowcase: dependencies/glew/src/glew-build/lib/libglew.a
bin/Ex_GraphicShowcase: /usr/lib/libassimp.so
bin/Ex_GraphicShowcase: dependencies/OpenAL/src/OpenAL-build/libopenal.so
bin/Ex_GraphicShowcase: dependencies/TinyXML/src/TinyXML-build/libtinyxml2.so
bin/Ex_GraphicShowcase: lib/libGeKo_Gameplay.a
bin/Ex_GraphicShowcase: lib/libGeKo_Physics.a
bin/Ex_GraphicShowcase: lib/libLogging.a
bin/Ex_GraphicShowcase: lib/libGeKo_Sound.a
bin/Ex_GraphicShowcase: lib/libGeKo_Graphics.a
bin/Ex_GraphicShowcase: lib/libGeKo_Debugger.a
bin/Ex_GraphicShowcase: /usr/lib/x86_64-linux-gnu/libSM.so
bin/Ex_GraphicShowcase: /usr/lib/x86_64-linux-gnu/libICE.so
bin/Ex_GraphicShowcase: /usr/lib/x86_64-linux-gnu/libX11.so
bin/Ex_GraphicShowcase: /usr/lib/x86_64-linux-gnu/libXext.so
bin/Ex_GraphicShowcase: dependencies/glfw/src/glfw-build/src/libglfw3.a
bin/Ex_GraphicShowcase: dependencies/glew/src/glew-build/lib/libglew.a
bin/Ex_GraphicShowcase: /usr/lib/libassimp.so
bin/Ex_GraphicShowcase: dependencies/OpenAL/src/OpenAL-build/libopenal.so
bin/Ex_GraphicShowcase: dependencies/TinyXML/src/TinyXML-build/libtinyxml2.so
bin/Ex_GraphicShowcase: src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../../bin/Ex_GraphicShowcase"
	cd /home/maik/projects/lisa_geko/GeKo/build/src/executables/Ex_GraphicShowcase && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Ex_GraphicShowcase.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/build: bin/Ex_GraphicShowcase
.PHONY : src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/build

src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/requires: src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/main.cpp.o.requires
.PHONY : src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/requires

src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/clean:
	cd /home/maik/projects/lisa_geko/GeKo/build/src/executables/Ex_GraphicShowcase && $(CMAKE_COMMAND) -P CMakeFiles/Ex_GraphicShowcase.dir/cmake_clean.cmake
.PHONY : src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/clean

src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/depend:
	cd /home/maik/projects/lisa_geko/GeKo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maik/projects/lisa_geko/GeKo /home/maik/projects/lisa_geko/GeKo/src/executables/Ex_GraphicShowcase /home/maik/projects/lisa_geko/GeKo/build /home/maik/projects/lisa_geko/GeKo/build/src/executables/Ex_GraphicShowcase /home/maik/projects/lisa_geko/GeKo/build/src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/executables/Ex_GraphicShowcase/CMakeFiles/Ex_GraphicShowcase.dir/depend

