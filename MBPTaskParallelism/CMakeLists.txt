cmake_minimum_required(VERSION 3.17)
project(MBPTaskParallelism)

set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

set(CMAKE_CXX_STANDARD 20)

add_executable(MBPTaskParallelism main.cpp)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(MBPTaskParallelism PUBLIC OpenMP::OpenMP_CXX)
endif()
