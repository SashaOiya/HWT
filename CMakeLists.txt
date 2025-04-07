cmake_minimum_required(VERSION 3.11)

project(hwt LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)
find_package(CLI11 REQUIRED)
enable_testing()

foreach(TREE_PROJECT set hwt)
    add_executable(${TREE_PROJECT} src/driver.cpp)
    target_compile_features(${TREE_PROJECT} PUBLIC cxx_std_20)
    target_include_directories(${TREE_PROJECT} PRIVATE ./includes)
    target_link_libraries(${TREE_PROJECT} PRIVATE CLI11::CLI11)
endforeach()

add_subdirectory(test/e2e_tests)
add_subdirectory(test/unit_tests)
add_subdirectory(benchmark)
