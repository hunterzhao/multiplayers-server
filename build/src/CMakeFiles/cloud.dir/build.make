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
CMAKE_SOURCE_DIR = /mnt/shared/game/server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/shared/game/server/build

# Include any dependencies generated for this target.
include src/CMakeFiles/cloud.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/cloud.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/cloud.dir/flags.make

src/CMakeFiles/cloud.dir/tcpserver.cpp.o: src/CMakeFiles/cloud.dir/flags.make
src/CMakeFiles/cloud.dir/tcpserver.cpp.o: ../src/tcpserver.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/shared/game/server/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/cloud.dir/tcpserver.cpp.o"
	cd /mnt/shared/game/server/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/cloud.dir/tcpserver.cpp.o -c /mnt/shared/game/server/src/tcpserver.cpp

src/CMakeFiles/cloud.dir/tcpserver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cloud.dir/tcpserver.cpp.i"
	cd /mnt/shared/game/server/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /mnt/shared/game/server/src/tcpserver.cpp > CMakeFiles/cloud.dir/tcpserver.cpp.i

src/CMakeFiles/cloud.dir/tcpserver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cloud.dir/tcpserver.cpp.s"
	cd /mnt/shared/game/server/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /mnt/shared/game/server/src/tcpserver.cpp -o CMakeFiles/cloud.dir/tcpserver.cpp.s

src/CMakeFiles/cloud.dir/tcpserver.cpp.o.requires:
.PHONY : src/CMakeFiles/cloud.dir/tcpserver.cpp.o.requires

src/CMakeFiles/cloud.dir/tcpserver.cpp.o.provides: src/CMakeFiles/cloud.dir/tcpserver.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/cloud.dir/build.make src/CMakeFiles/cloud.dir/tcpserver.cpp.o.provides.build
.PHONY : src/CMakeFiles/cloud.dir/tcpserver.cpp.o.provides

src/CMakeFiles/cloud.dir/tcpserver.cpp.o.provides.build: src/CMakeFiles/cloud.dir/tcpserver.cpp.o

# Object files for target cloud
cloud_OBJECTS = \
"CMakeFiles/cloud.dir/tcpserver.cpp.o"

# External object files for target cloud
cloud_EXTERNAL_OBJECTS =

src/libcloud.a: src/CMakeFiles/cloud.dir/tcpserver.cpp.o
src/libcloud.a: src/CMakeFiles/cloud.dir/build.make
src/libcloud.a: src/CMakeFiles/cloud.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libcloud.a"
	cd /mnt/shared/game/server/build/src && $(CMAKE_COMMAND) -P CMakeFiles/cloud.dir/cmake_clean_target.cmake
	cd /mnt/shared/game/server/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cloud.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/cloud.dir/build: src/libcloud.a
.PHONY : src/CMakeFiles/cloud.dir/build

src/CMakeFiles/cloud.dir/requires: src/CMakeFiles/cloud.dir/tcpserver.cpp.o.requires
.PHONY : src/CMakeFiles/cloud.dir/requires

src/CMakeFiles/cloud.dir/clean:
	cd /mnt/shared/game/server/build/src && $(CMAKE_COMMAND) -P CMakeFiles/cloud.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/cloud.dir/clean

src/CMakeFiles/cloud.dir/depend:
	cd /mnt/shared/game/server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/shared/game/server /mnt/shared/game/server/src /mnt/shared/game/server/build /mnt/shared/game/server/build/src /mnt/shared/game/server/build/src/CMakeFiles/cloud.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/cloud.dir/depend
