add_executable(hwt avl_tree.cpp)
add_executable(set std_set.cpp)

target_include_directories(hwt PUBLIC ${PROJECT_SOURCE_DIR}/includes )

option(DUMP "Builds a tree graph" OFF)
if(DUMP)
	target_compile_definitions(hwt PRIVATE DUMP)
endif()
