add_executable(hwt_unit_test tests.cpp)

target_link_libraries(hwt_unit_test 
PRIVATE
	GTest::GTest
	GTest::gtest_main
)

target_include_directories(hwt_unit_test PUBLIC ${PROJECT_SOURCE_DIR}/includes)

include(GoogleTest)
gtest_discover_tests(hwt_unit_test)