cmake_minimum_required(VERSION 3.17)
project(TryMPI)

set(CMAKE_C_COMPILER /usr/local/bin/mpicxx)
set(CMAKE_CXX_COMPILER /usr/local/bin/mpicxx)

set(CMAKE_CXX_STANDARD 20)

add_executable(TryMPI main.cpp)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(TryMPI PUBLIC OpenMP::OpenMP_CXX)
endif()