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
CMAKE_SOURCE_DIR = /home/maik/projects/lisa_geko/GeKo/build/dependencies/TinyXML/src/TinyXML

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/maik/projects/lisa_geko/GeKo/build/dependencies/TinyXML/src/TinyXML-build

# Utility rule file for DATA_COPY.

# Include the progress variables for this target.
include CMakeFiles/DATA_COPY.dir/progress.make

CMakeFiles/DATA_COPY:
	/usr/bin/cmake -E copy /home/maik/projects/lisa_geko/GeKo/build/dependencies/TinyXML/src/TinyXML/resources/dream.xml /home/maik/projects/lisa_geko/GeKo/build/dependencies/TinyXML/src/TinyXML-build/resources/
	/usr/bin/cmake -E copy /home/maik/projects/lisa_geko/GeKo/build/dependencies/TinyXML/src/TinyXML/resources/empty.xml /home/maik/projects/lisa_geko/GeKo/build/dependencies/TinyXML/src/TinyXML-build/resources/
	/usr/bin/cmake -E copy /home/maik/projects/lisa_geko/GeKo/build/dependencies/TinyXML/src/TinyXML/resources/utf8test.xml /home/maik/projects/lisa_geko/GeKo/build/dependencies/TinyXML/src/TinyXML-build/resources/
	/usr/bin/cmake -E copy /home/maik/projects/lisa_geko/GeKo/build/dependencies/TinyXML/src/TinyXML/resources/utf8testverify.xml /home/maik/projects/lisa_geko/GeKo/build/dependencies/TinyXML/src/TinyXML-build/resources/

DATA_COPY: CMakeFiles/DATA_COPY
DATA_COPY: CMakeFiles/DATA_COPY.dir/build.make
.PHONY : DATA_COPY

# Rule to build all files generated by this target.
CMakeFiles/DATA_COPY.dir/build: DATA_COPY
.PHONY : CMakeFiles/DATA_COPY.dir/build

CMakeFiles/DATA_COPY.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DATA_COPY.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DATA_COPY.dir/clean

CMakeFiles/DATA_COPY.dir/depend:
	cd /home/maik/projects/lisa_geko/GeKo/build/dependencies/TinyXML/src/TinyXML-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maik/projects/lisa_geko/GeKo/build/dependencies/TinyXML/src/TinyXML /home/maik/projects/lisa_geko/GeKo/build/dependencies/TinyXML/src/TinyXML /home/maik/projects/lisa_geko/GeKo/build/dependencies/TinyXML/src/TinyXML-build /home/maik/projects/lisa_geko/GeKo/build/dependencies/TinyXML/src/TinyXML-build /home/maik/projects/lisa_geko/GeKo/build/dependencies/TinyXML/src/TinyXML-build/CMakeFiles/DATA_COPY.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/DATA_COPY.dir/depend

