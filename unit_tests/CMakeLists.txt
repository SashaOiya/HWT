add_executable(hwt_unit_test
			   tests.cpp)

set(TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../")

set_target_properties(hwt_unit_test PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

target_compile_definitions(hwt_unit_test PUBLIC
						   -DTEST_DATA_DIR="${TEST_DATA_DIR}")

target_link_libraries(hwt_unit_test PRIVATE
					  GTest::GTest
					  GTest::gtest_main)

target_include_directories(hwt_unit_test PUBLIC
	                       ${PROJECT_SOURCE_DIR}/includes
						   ${PROJECT_SOURCE_DIR}/unit_tests)

include(GoogleTest)
gtest_discover_tests(hwt_unit_test)