cmake_minimum_required(VERSION 3.17)
project(MBPSequential)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra")

add_executable(MBPSequential main.cpp EdgeListGraph.h Finder.h read.h Edge.h State.h Color.h AdjacencyListGraph.h utils.h)