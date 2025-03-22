cmake_minimum_required(VERSION 3.11)

project(hwt LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)
find_package(CLI11 REQUIRED)
enable_testing()

add_executable(set src/driver.cpp)
target_compile_features(set PUBLIC cxx_std_23)
target_include_directories(set PRIVATE ./includes)
target_link_libraries(set PRIVATE CLI11::CLI11)

add_executable(hwt src/driver.cpp)
target_compile_features(hwt PUBLIC cxx_std_23)
target_include_directories(hwt PRIVATE ./includes)
target_compile_definitions(hwt PRIVATE AVL_TREE)
target_link_libraries(hwt PRIVATE CLI11::CLI11)

add_subdirectory(test/e2e_tests)
add_subdirectory(test/unit_tests)
add_subdirectory(benchmark)
