cmake_minimum_required(VERSION 3.11)

project(hwt)

set ( CXX_STANDARD 20 )
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(DUMP "Builds a tree graph" OFF)

if(DUMP)
	add_definitions(-DDUMP)
endif()

find_package(GTest REQUIRED)
enable_testing()
add_subdirectory(e2e_tests)
add_subdirectory(unit_tests)

add_executable(hwt ./hwt.cpp)