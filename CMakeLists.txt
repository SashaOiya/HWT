cmake_minimum_required(VERSION 3.11)

project(hwt LANGUAGES CXX)

add_executable(hwt src/driver.cpp)
target_include_directories(hwt PUBLIC ${PROJECT_SOURCE_DIR}/includes)
target_compile_definitions(hwt PRIVATE AVL_TREE)

option(DUMP "Builds a tree graph" OFF)
if(DUMP)
	target_compile_definitions(hwt PRIVATE DUMP)
endif()

add_executable(set src/driver.cpp)
target_include_directories(set PUBLIC ${PROJECT_SOURCE_DIR}/includes)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)
enable_testing()
add_subdirectory(test/e2e_tests)
add_subdirectory(test/unit_tests)
add_subdirectory(benchmark)