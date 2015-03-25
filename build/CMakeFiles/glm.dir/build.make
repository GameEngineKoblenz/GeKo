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

# Utility rule file for glm.

# Include the progress variables for this target.
include CMakeFiles/glm.dir/progress.make

CMakeFiles/glm: CMakeFiles/./glm-complete

CMakeFiles/./glm-complete: dependencies/glm/src/glm-stamp/./glm-install
CMakeFiles/./glm-complete: dependencies/glm/src/glm-stamp/./glm-mkdir
CMakeFiles/./glm-complete: dependencies/glm/src/glm-stamp/./glm-download
CMakeFiles/./glm-complete: dependencies/glm/src/glm-stamp/./glm-update
CMakeFiles/./glm-complete: dependencies/glm/src/glm-stamp/./glm-patch
CMakeFiles/./glm-complete: dependencies/glm/src/glm-stamp/./glm-configure
CMakeFiles/./glm-complete: dependencies/glm/src/glm-stamp/./glm-build
CMakeFiles/./glm-complete: dependencies/glm/src/glm-stamp/./glm-install
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Completed 'glm'"
	/usr/bin/cmake -E make_directory /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles/.
	/usr/bin/cmake -E touch /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles/./glm-complete
	/usr/bin/cmake -E touch /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src/glm-stamp/./glm-done

dependencies/glm/src/glm-stamp/./glm-install: dependencies/glm/src/glm-stamp/./glm-build
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "No install step for 'glm'"
	cd /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src/glm-build && /usr/bin/cmake -E touch /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src/glm-stamp/./glm-install

dependencies/glm/src/glm-stamp/./glm-mkdir:
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Creating directories for 'glm'"
	/usr/bin/cmake -E make_directory /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src/glm
	/usr/bin/cmake -E make_directory /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src/glm-build
	/usr/bin/cmake -E make_directory /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm
	/usr/bin/cmake -E make_directory /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/tmp
	/usr/bin/cmake -E make_directory /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src/glm-stamp/.
	/usr/bin/cmake -E make_directory /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src
	/usr/bin/cmake -E touch /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src/glm-stamp/./glm-mkdir

dependencies/glm/src/glm-stamp/./glm-download: dependencies/glm/src/glm-stamp/glm-gitinfo.txt
dependencies/glm/src/glm-stamp/./glm-download: dependencies/glm/src/glm-stamp/./glm-mkdir
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Performing download step (git clone) for 'glm'"
	cd /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src && /usr/bin/cmake -P /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/tmp/glm-gitclone.cmake
	cd /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src && /usr/bin/cmake -E touch /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src/glm-stamp/./glm-download

dependencies/glm/src/glm-stamp/./glm-update: dependencies/glm/src/glm-stamp/./glm-download
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "No update step for 'glm'"
	cd /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src/glm && /usr/bin/cmake -E touch /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src/glm-stamp/./glm-update

dependencies/glm/src/glm-stamp/./glm-patch: dependencies/glm/src/glm-stamp/./glm-download
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "No patch step for 'glm'"
	/usr/bin/cmake -E touch /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src/glm-stamp/./glm-patch

dependencies/glm/src/glm-stamp/./glm-configure: dependencies/glm/tmp/glm-cfgcmd.txt
dependencies/glm/src/glm-stamp/./glm-configure: dependencies/glm/src/glm-stamp/./glm-update
dependencies/glm/src/glm-stamp/./glm-configure: dependencies/glm/src/glm-stamp/./glm-patch
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Performing configure step for 'glm'"
	cd /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src/glm-build && /usr/bin/cmake -DCMAKE_INSTALL_PREFIX=/home/maik/projects/lisa_geko/GeKo/build/dependencies/glm "-GUnix Makefiles" /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src/glm
	cd /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src/glm-build && /usr/bin/cmake -E touch /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src/glm-stamp/./glm-configure

dependencies/glm/src/glm-stamp/./glm-build: dependencies/glm/src/glm-stamp/./glm-configure
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Performing build step for 'glm'"
	cd /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src/glm-build && $(MAKE)
	cd /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src/glm-build && /usr/bin/cmake -E touch /home/maik/projects/lisa_geko/GeKo/build/dependencies/glm/src/glm-stamp/./glm-build

glm: CMakeFiles/glm
glm: CMakeFiles/./glm-complete
glm: dependencies/glm/src/glm-stamp/./glm-install
glm: dependencies/glm/src/glm-stamp/./glm-mkdir
glm: dependencies/glm/src/glm-stamp/./glm-download
glm: dependencies/glm/src/glm-stamp/./glm-update
glm: dependencies/glm/src/glm-stamp/./glm-patch
glm: dependencies/glm/src/glm-stamp/./glm-configure
glm: dependencies/glm/src/glm-stamp/./glm-build
glm: CMakeFiles/glm.dir/build.make
.PHONY : glm

# Rule to build all files generated by this target.
CMakeFiles/glm.dir/build: glm
.PHONY : CMakeFiles/glm.dir/build

CMakeFiles/glm.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/glm.dir/cmake_clean.cmake
.PHONY : CMakeFiles/glm.dir/clean

CMakeFiles/glm.dir/depend:
	cd /home/maik/projects/lisa_geko/GeKo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maik/projects/lisa_geko/GeKo /home/maik/projects/lisa_geko/GeKo /home/maik/projects/lisa_geko/GeKo/build /home/maik/projects/lisa_geko/GeKo/build /home/maik/projects/lisa_geko/GeKo/build/CMakeFiles/glm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/glm.dir/depend

