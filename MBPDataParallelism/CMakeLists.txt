cmake_minimum_required(VERSION 3.17)
project(MBPDataParallelism)

set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra")

set(CMAKE_CXX_STANDARD 20)

add_executable(MBPDataParallelism main.cpp EdgeListGraph.h Finder.h read.h Edge.h State.h Color.h AdjacencyListGraph.h utils.h Explorer.h)

find_package(OpenMP REQUIRED)
target_link_libraries(MBPDataParallelism PUBLIC OpenMP::OpenMP_CXX)

