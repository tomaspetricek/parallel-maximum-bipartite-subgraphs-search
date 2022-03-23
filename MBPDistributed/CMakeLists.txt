cmake_minimum_required(VERSION 3.17)
project(MBPDistributed)

set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra")

set(CMAKE_CXX_STANDARD 20)

add_executable(MBPDistributed main.cpp edge_list.h finder.h read.h edge.h state.h color.h adjacency_list.h utils.h explorer.h master.h slave.h process.h)

find_package(OpenMP REQUIRED)
find_package( Boost REQUIRED COMPONENTS mpi serialization)

include_directories( ${Boost_INCLUDE_DIRS} )

target_link_libraries(MBPDistributed PUBLIC ${Boost_LIBRARIES} OpenMP::OpenMP_CXX)