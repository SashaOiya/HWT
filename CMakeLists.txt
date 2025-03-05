cmake_minimum_required(VERSION 3.11)

project(hwt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20 )
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)
enable_testing()
add_subdirectory(test/e2e_tests)
add_subdirectory(test/unit_tests)
add_subdirectory(benchmark)
add_subdirectory(src)