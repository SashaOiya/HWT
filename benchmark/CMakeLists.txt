add_executable(hwt_benchmark
			   bench.cpp)

target_compile_options(hwt_benchmark PRIVATE -O3)

target_link_libraries(hwt_benchmark
PRIVATE
    benchmark::benchmark
)

target_include_directories(hwt_benchmark PUBLIC ${PROJECT_SOURCE_DIR}/includes)
target_include_directories(hwt_benchmark PUBLIC ${PROJECT_SOURCE_DIR}/src)
