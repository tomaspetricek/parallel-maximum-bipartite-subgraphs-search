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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.22.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.22.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDataParallelism

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDataParallelism/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/MBPDataParallelism.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/MBPDataParallelism.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/MBPDataParallelism.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MBPDataParallelism.dir/flags.make

CMakeFiles/MBPDataParallelism.dir/main.cpp.o: CMakeFiles/MBPDataParallelism.dir/flags.make
CMakeFiles/MBPDataParallelism.dir/main.cpp.o: ../main.cpp
CMakeFiles/MBPDataParallelism.dir/main.cpp.o: CMakeFiles/MBPDataParallelism.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDataParallelism/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MBPDataParallelism.dir/main.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MBPDataParallelism.dir/main.cpp.o -MF CMakeFiles/MBPDataParallelism.dir/main.cpp.o.d -o CMakeFiles/MBPDataParallelism.dir/main.cpp.o -c /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDataParallelism/main.cpp

CMakeFiles/MBPDataParallelism.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MBPDataParallelism.dir/main.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDataParallelism/main.cpp > CMakeFiles/MBPDataParallelism.dir/main.cpp.i

CMakeFiles/MBPDataParallelism.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MBPDataParallelism.dir/main.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDataParallelism/main.cpp -o CMakeFiles/MBPDataParallelism.dir/main.cpp.s

# Object files for target MBPDataParallelism
MBPDataParallelism_OBJECTS = \
"CMakeFiles/MBPDataParallelism.dir/main.cpp.o"

# External object files for target MBPDataParallelism
MBPDataParallelism_EXTERNAL_OBJECTS =

MBPDataParallelism: CMakeFiles/MBPDataParallelism.dir/main.cpp.o
MBPDataParallelism: CMakeFiles/MBPDataParallelism.dir/build.make
MBPDataParallelism: /usr/local/lib/libomp.dylib
MBPDataParallelism: CMakeFiles/MBPDataParallelism.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDataParallelism/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable MBPDataParallelism"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MBPDataParallelism.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MBPDataParallelism.dir/build: MBPDataParallelism
.PHONY : CMakeFiles/MBPDataParallelism.dir/build

CMakeFiles/MBPDataParallelism.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MBPDataParallelism.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MBPDataParallelism.dir/clean

CMakeFiles/MBPDataParallelism.dir/depend:
	cd /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDataParallelism/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDataParallelism /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDataParallelism /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDataParallelism/cmake-build-debug /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDataParallelism/cmake-build-debug /Users/tomaspetricek/CVUT/CVUT-2021_2022/letni_semestr/pdp/pdp/MBPDataParallelism/cmake-build-debug/CMakeFiles/MBPDataParallelism.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MBPDataParallelism.dir/depend

