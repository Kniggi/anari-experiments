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

# Include any dependencies generated for this target.
include volkit/src/examples/CMakeFiles/FileIOCPP.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include volkit/src/examples/CMakeFiles/FileIOCPP.dir/compiler_depend.make

# Include the progress variables for this target.
include volkit/src/examples/CMakeFiles/FileIOCPP.dir/progress.make

# Include the compile flags for this target's objects.
include volkit/src/examples/CMakeFiles/FileIOCPP.dir/flags.make

volkit/src/examples/CMakeFiles/FileIOCPP.dir/FileIO.cpp.o: volkit/src/examples/CMakeFiles/FileIOCPP.dir/flags.make
volkit/src/examples/CMakeFiles/FileIOCPP.dir/FileIO.cpp.o: ../volkit/src/examples/FileIO.cpp
volkit/src/examples/CMakeFiles/FileIOCPP.dir/FileIO.cpp.o: volkit/src/examples/CMakeFiles/FileIOCPP.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/niklas/Schreibtisch/gitrepo/anari-experiments/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object volkit/src/examples/CMakeFiles/FileIOCPP.dir/FileIO.cpp.o"
	cd /home/niklas/Schreibtisch/gitrepo/anari-experiments/build/volkit/src/examples && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT volkit/src/examples/CMakeFiles/FileIOCPP.dir/FileIO.cpp.o -MF CMakeFiles/FileIOCPP.dir/FileIO.cpp.o.d -o CMakeFiles/FileIOCPP.dir/FileIO.cpp.o -c /home/niklas/Schreibtisch/gitrepo/anari-experiments/volkit/src/examples/FileIO.cpp

volkit/src/examples/CMakeFiles/FileIOCPP.dir/FileIO.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FileIOCPP.dir/FileIO.cpp.i"
	cd /home/niklas/Schreibtisch/gitrepo/anari-experiments/build/volkit/src/examples && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/niklas/Schreibtisch/gitrepo/anari-experiments/volkit/src/examples/FileIO.cpp > CMakeFiles/FileIOCPP.dir/FileIO.cpp.i

volkit/src/examples/CMakeFiles/FileIOCPP.dir/FileIO.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FileIOCPP.dir/FileIO.cpp.s"
	cd /home/niklas/Schreibtisch/gitrepo/anari-experiments/build/volkit/src/examples && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/niklas/Schreibtisch/gitrepo/anari-experiments/volkit/src/examples/FileIO.cpp -o CMakeFiles/FileIOCPP.dir/FileIO.cpp.s

# Object files for target FileIOCPP
FileIOCPP_OBJECTS = \
"CMakeFiles/FileIOCPP.dir/FileIO.cpp.o"

# External object files for target FileIOCPP
FileIOCPP_EXTERNAL_OBJECTS =

volkit/src/examples/FileIOCPP: volkit/src/examples/CMakeFiles/FileIOCPP.dir/FileIO.cpp.o
volkit/src/examples/FileIOCPP: volkit/src/examples/CMakeFiles/FileIOCPP.dir/build.make
volkit/src/examples/FileIOCPP: /usr/local/cuda-11.5/lib64/libcudart_static.a
volkit/src/examples/FileIOCPP: /usr/lib/x86_64-linux-gnu/librt.so
volkit/src/examples/FileIOCPP: volkit/src/vkt/libvolkit.a
volkit/src/examples/FileIOCPP: /usr/local/lib/libboost_filesystem.so.1.77.0
volkit/src/examples/FileIOCPP: /usr/local/lib/libboost_atomic.so.1.77.0
volkit/src/examples/FileIOCPP: /usr/local/lib/libboost_system.so.1.77.0
volkit/src/examples/FileIOCPP: /usr/local/lib/libboost_serialization.so.1.77.0
volkit/src/examples/FileIOCPP: /usr/lib/libniftiio.so
volkit/src/examples/FileIOCPP: /usr/lib/libznz.so
volkit/src/examples/FileIOCPP: /usr/lib/gcc/x86_64-linux-gnu/9/libgomp.so
volkit/src/examples/FileIOCPP: /usr/lib/x86_64-linux-gnu/libpthread.so
volkit/src/examples/FileIOCPP: volkit/src/3rdparty/visionaray/src/common/libvisionaray_common.a
volkit/src/examples/FileIOCPP: /usr/lib/x86_64-linux-gnu/libglut.so
volkit/src/examples/FileIOCPP: /usr/lib/x86_64-linux-gnu/libXmu.so
volkit/src/examples/FileIOCPP: /usr/lib/x86_64-linux-gnu/libXi.so
volkit/src/examples/FileIOCPP: volkit/src/3rdparty/visionaray/src/visionaray/libvisionaray.a
volkit/src/examples/FileIOCPP: /usr/lib/x86_64-linux-gnu/libGLEW.so
volkit/src/examples/FileIOCPP: /usr/lib/x86_64-linux-gnu/libGL.so
volkit/src/examples/FileIOCPP: /usr/lib/x86_64-linux-gnu/libGLU.so
volkit/src/examples/FileIOCPP: /usr/local/lib/libboost_filesystem.so.1.77.0
volkit/src/examples/FileIOCPP: /usr/local/lib/libboost_atomic.so.1.77.0
volkit/src/examples/FileIOCPP: /usr/local/lib/libboost_iostreams.so.1.77.0
volkit/src/examples/FileIOCPP: /usr/local/lib/libboost_system.so.1.77.0
volkit/src/examples/FileIOCPP: /usr/lib/x86_64-linux-gnu/libpng.so
volkit/src/examples/FileIOCPP: /usr/local/cuda-11.5/lib64/libcudart_static.a
volkit/src/examples/FileIOCPP: /usr/lib/x86_64-linux-gnu/librt.so
volkit/src/examples/FileIOCPP: /usr/lib/x86_64-linux-gnu/libz.so
volkit/src/examples/FileIOCPP: libpbrtParser.a
volkit/src/examples/FileIOCPP: volkit/src/examples/CMakeFiles/FileIOCPP.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/niklas/Schreibtisch/gitrepo/anari-experiments/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable FileIOCPP"
	cd /home/niklas/Schreibtisch/gitrepo/anari-experiments/build/volkit/src/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FileIOCPP.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
volkit/src/examples/CMakeFiles/FileIOCPP.dir/build: volkit/src/examples/FileIOCPP
.PHONY : volkit/src/examples/CMakeFiles/FileIOCPP.dir/build

volkit/src/examples/CMakeFiles/FileIOCPP.dir/clean:
	cd /home/niklas/Schreibtisch/gitrepo/anari-experiments/build/volkit/src/examples && $(CMAKE_COMMAND) -P CMakeFiles/FileIOCPP.dir/cmake_clean.cmake
.PHONY : volkit/src/examples/CMakeFiles/FileIOCPP.dir/clean

volkit/src/examples/CMakeFiles/FileIOCPP.dir/depend:
	cd /home/niklas/Schreibtisch/gitrepo/anari-experiments/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/niklas/Schreibtisch/gitrepo/anari-experiments /home/niklas/Schreibtisch/gitrepo/anari-experiments/volkit/src/examples /home/niklas/Schreibtisch/gitrepo/anari-experiments/build /home/niklas/Schreibtisch/gitrepo/anari-experiments/build/volkit/src/examples /home/niklas/Schreibtisch/gitrepo/anari-experiments/build/volkit/src/examples/CMakeFiles/FileIOCPP.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : volkit/src/examples/CMakeFiles/FileIOCPP.dir/depend

