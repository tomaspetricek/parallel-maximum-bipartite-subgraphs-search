cmake_minimum_required(VERSION 3.17)
project(TryBoostMPI)

set(CMAKE_CXX_STANDARD 20)

add_executable(TryBoostMPI main.cpp)

find_package( Boost REQUIRED COMPONENTS mpi serialization)
include_directories( ${Boost_INCLUDE_DIRS} )

find_package(OpenMP REQUIRED)
target_link_libraries(TryBoostMPI PUBLIC ${Boost_LIBRARIES})
