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

# Utility rule file for OpenAL.

# Include the progress variables for this target.
include CMakeFiles/OpenAL.dir/progress.make

CMakeFiles/OpenAL: CMakeFiles/./OpenAL-complete

CMakeFiles/./OpenAL-complete: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-install
CMakeFiles/./OpenAL-complete: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-mkdir
CMakeFiles/./OpenAL-complete: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-download
CMakeFiles/./OpenAL-complete: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-update
CMakeFiles/./OpenAL-complete: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-patch
CMakeFiles/./OpenAL-complete: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-configure
CMakeFiles/./OpenAL-complete: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-build
CMakeFiles/./OpenAL-complete: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-install
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Completed 'OpenAL'"
	/usr/bin/cmake -E make_directory /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles/.
	/usr/bin/cmake -E touch /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles/./OpenAL-complete
	/usr/bin/cmake -E touch /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-done

dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-install: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-build
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "No install step for 'OpenAL'"
	cd /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src/OpenAL-build && /usr/bin/cmake -E touch /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-install

dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-mkdir:
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Creating directories for 'OpenAL'"
	/usr/bin/cmake -E make_directory /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src/OpenAL
	/usr/bin/cmake -E make_directory /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src/OpenAL-build
	/usr/bin/cmake -E make_directory /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL
	/usr/bin/cmake -E make_directory /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/tmp
	/usr/bin/cmake -E make_directory /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src/OpenAL-stamp/.
	/usr/bin/cmake -E make_directory /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src
	/usr/bin/cmake -E touch /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-mkdir

dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-download: dependencies/OpenAL/src/OpenAL-stamp/OpenAL-gitinfo.txt
dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-download: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-mkdir
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Performing download step (git clone) for 'OpenAL'"
	cd /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src && /usr/bin/cmake -P /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/tmp/OpenAL-gitclone.cmake
	cd /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src && /usr/bin/cmake -E touch /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-download

dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-update: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-download
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "No update step for 'OpenAL'"
	cd /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src/OpenAL && /usr/bin/cmake -E touch /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-update

dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-patch: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-download
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "No patch step for 'OpenAL'"
	/usr/bin/cmake -E touch /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-patch

dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-configure: dependencies/OpenAL/tmp/OpenAL-cfgcmd.txt
dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-configure: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-update
dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-configure: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-patch
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Performing configure step for 'OpenAL'"
	cd /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src/OpenAL-build && /usr/bin/cmake -DCMAKE_INSTALL_PREFIX=/home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL "-GUnix Makefiles" /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src/OpenAL
	cd /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src/OpenAL-build && /usr/bin/cmake -E touch /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-configure

dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-build: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-configure
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Performing build step for 'OpenAL'"
	cd /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src/OpenAL-build && $(MAKE)
	cd /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src/OpenAL-build && /usr/bin/cmake -E touch /home/maik/projects/lisa_geko/GeKo/build/dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-build

OpenAL: CMakeFiles/OpenAL
OpenAL: CMakeFiles/./OpenAL-complete
OpenAL: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-install
OpenAL: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-mkdir
OpenAL: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-download
OpenAL: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-update
OpenAL: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-patch
OpenAL: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-configure
OpenAL: dependencies/OpenAL/src/OpenAL-stamp/./OpenAL-build
OpenAL: CMakeFiles/OpenAL.dir/build.make
.PHONY : OpenAL

# Rule to build all files generated by this target.
CMakeFiles/OpenAL.dir/build: OpenAL
.PHONY : CMakeFiles/OpenAL.dir/build

CMakeFiles/OpenAL.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/OpenAL.dir/cmake_clean.cmake
.PHONY : CMakeFiles/OpenAL.dir/clean

CMakeFiles/OpenAL.dir/depend:
	cd /home/maik/projects/lisa_geko/GeKo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maik/projects/lisa_geko/GeKo /home/maik/projects/lisa_geko/GeKo /home/maik/projects/lisa_geko/GeKo/build /home/maik/projects/lisa_geko/GeKo/build /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles/OpenAL.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/OpenAL.dir/depend

