# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/niklas/Schreibtisch/gitrepo/anari-experiments

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/niklas/Schreibtisch/gitrepo/anari-experiments/build

# Utility rule file for NightlyCoverage.

# Include any custom commands dependencies for this target.
include volkit/src/3rdparty/visionaray/CMakeFiles/NightlyCoverage.dir/compiler_depend.make

# Include the progress variables for this target.
include volkit/src/3rdparty/visionaray/CMakeFiles/NightlyCoverage.dir/progress.make

volkit/src/3rdparty/visionaray/CMakeFiles/NightlyCoverage:
	cd /home/niklas/Schreibtisch/gitrepo/anari-experiments/build/volkit/src/3rdparty/visionaray && /usr/local/bin/ctest -D NightlyCoverage

NightlyCoverage: volkit/src/3rdparty/visionaray/CMakeFiles/NightlyCoverage
NightlyCoverage: volkit/src/3rdparty/visionaray/CMakeFiles/NightlyCoverage.dir/build.make
.PHONY : NightlyCoverage

# Rule to build all files generated by this target.
volkit/src/3rdparty/visionaray/CMakeFiles/NightlyCoverage.dir/build: NightlyCoverage
.PHONY : volkit/src/3rdparty/visionaray/CMakeFiles/NightlyCoverage.dir/build

volkit/src/3rdparty/visionaray/CMakeFiles/NightlyCoverage.dir/clean:
	cd /home/niklas/Schreibtisch/gitrepo/anari-experiments/build/volkit/src/3rdparty/visionaray && $(CMAKE_COMMAND) -P CMakeFiles/NightlyCoverage.dir/cmake_clean.cmake
.PHONY : volkit/src/3rdparty/visionaray/CMakeFiles/NightlyCoverage.dir/clean

volkit/src/3rdparty/visionaray/CMakeFiles/NightlyCoverage.dir/depend:
	cd /home/niklas/Schreibtisch/gitrepo/anari-experiments/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/niklas/Schreibtisch/gitrepo/anari-experiments /home/niklas/Schreibtisch/gitrepo/anari-experiments/volkit/src/3rdparty/visionaray /home/niklas/Schreibtisch/gitrepo/anari-experiments/build /home/niklas/Schreibtisch/gitrepo/anari-experiments/build/volkit/src/3rdparty/visionaray /home/niklas/Schreibtisch/gitrepo/anari-experiments/build/volkit/src/3rdparty/visionaray/CMakeFiles/NightlyCoverage.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : volkit/src/3rdparty/visionaray/CMakeFiles/NightlyCoverage.dir/depend

