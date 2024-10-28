#include <fstream>
#include <string>

#include <gtest/gtest.h>

#include "hwt.hpp"
#include "funcs.hpp"

TEST(test1, test1)
{
	test_funcs::run_test("/test1.txt");
}

TEST(test2, test2)
{
	test_funcs::run_test("/test2.txt");
}

TEST(test3, test3)
{
	test_funcs::run_test("/test3.txt");
}
