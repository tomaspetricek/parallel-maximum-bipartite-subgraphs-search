cmake_minimum_required(VERSION 3.17)
project(TryOpenMP)

set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

set(CMAKE_CXX_STANDARD 20)

add_executable(TryOpenMP main.cpp)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(TryOpenMP PUBLIC OpenMP::OpenMP_CXX)
endif()