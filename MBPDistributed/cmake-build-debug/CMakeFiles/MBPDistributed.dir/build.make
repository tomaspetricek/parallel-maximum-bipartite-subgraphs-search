# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDistributed

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDistributed/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/MBPDistributed.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MBPDistributed.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MBPDistributed.dir/flags.make

CMakeFiles/MBPDistributed.dir/main.cpp.o: CMakeFiles/MBPDistributed.dir/flags.make
CMakeFiles/MBPDistributed.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDistributed/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MBPDistributed.dir/main.cpp.o"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MBPDistributed.dir/main.cpp.o -c /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDistributed/main.cpp

CMakeFiles/MBPDistributed.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MBPDistributed.dir/main.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDistributed/main.cpp > CMakeFiles/MBPDistributed.dir/main.cpp.i

CMakeFiles/MBPDistributed.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MBPDistributed.dir/main.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDistributed/main.cpp -o CMakeFiles/MBPDistributed.dir/main.cpp.s

# Object files for target MBPDistributed
MBPDistributed_OBJECTS = \
"CMakeFiles/MBPDistributed.dir/main.cpp.o"

# External object files for target MBPDistributed
MBPDistributed_EXTERNAL_OBJECTS =

MBPDistributed: CMakeFiles/MBPDistributed.dir/main.cpp.o
MBPDistributed: CMakeFiles/MBPDistributed.dir/build.make
MBPDistributed: /usr/local/lib/libboost_mpi-mt.dylib
MBPDistributed: /usr/local/lib/libboost_serialization-mt.dylib
MBPDistributed: /usr/local/Cellar/open-mpi/4.1.2/lib/libmpi.dylib
MBPDistributed: /usr/local/lib/libomp.dylib
MBPDistributed: CMakeFiles/MBPDistributed.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDistributed/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable MBPDistributed"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MBPDistributed.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MBPDistributed.dir/build: MBPDistributed

.PHONY : CMakeFiles/MBPDistributed.dir/build

CMakeFiles/MBPDistributed.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MBPDistributed.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MBPDistributed.dir/clean

CMakeFiles/MBPDistributed.dir/depend:
	cd /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDistributed/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDistributed /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDistributed /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDistributed/cmake-build-debug /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDistributed/cmake-build-debug /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDistributed/cmake-build-debug/CMakeFiles/MBPDistributed.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MBPDistributed.dir/depend

