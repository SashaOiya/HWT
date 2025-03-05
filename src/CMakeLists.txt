add_executable(hwt_src main.cpp)

target_include_directories(hwt_src PUBLIC ${PROJECT_SOURCE_DIR}/includes )

option(DUMP "Builds a tree graph" OFF)
if(DUMP)
	target_compile_definitions(hwt_src PRIVATE DUMP)
endif()
