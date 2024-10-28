cmake_minimum_required(VERSION 3.11)

project(hwt)

set ( CXX_STANDART 20 )
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GTest REQUIRED)
enable_testing()
add_subdirectory(unit_tests)

add_executable(hwt ./hwt.cpp)
#add_subdirectory(unit_tests)
#add_executable(tree ./tree.cpp)