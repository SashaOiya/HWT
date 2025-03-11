cmake_minimum_required(VERSION 3.11)

project(hwt LANGUAGES CXX)

add_executable(hwt src/avl_tree.cpp)
add_executable(set src/std_set.cpp)

target_include_directories(hwt PUBLIC ${PROJECT_SOURCE_DIR}/includes )

option(DUMP "Builds a tree graph" OFF)
if(DUMP)
	target_compile_definitions(hwt PRIVATE DUMP)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)
enable_testing()
add_subdirectory(test/e2e_tests)
add_subdirectory(test/unit_tests)
add_subdirectory(benchmark)