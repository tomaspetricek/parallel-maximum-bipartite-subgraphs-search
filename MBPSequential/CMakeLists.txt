cmake_minimum_required(VERSION 3.17)
project(MBPSequential)

set(CMAKE_CXX_STANDARD 20)

add_executable(MBPSequential main.cpp EdgeListGraph.h Finder.h read.h Edge.h State.h Color.h AdjencyListGraph.h utils.h)