cmake_minimum_required (VERSION 3.10)
project (lab2 VERSION 1.0.0)
set (CMAKE_CXX_COMPILER "g++")

set (CMAKE_CXX_FLAGS    "-Wall -std=c++2a")
find_package (OpenMP REQUIRED)

add_executable (openmp main.cpp)

target_link_libraries (openmp PUBLIC OpenMP::OpenMP_CXX)
