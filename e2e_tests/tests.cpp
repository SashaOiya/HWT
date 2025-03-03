#include <gtest/gtest.h>

#include <string>

#include "funcs.hpp"

TEST(test1, test1) { test_funcs::run_test("/test1.txt"); }

TEST(test2, test2) { test_funcs::run_test("/test2.txt"); }

TEST(test3, test3) { test_funcs::run_test("/test3.txt"); }

TEST(test4, test4) { test_funcs::run_test("/test4.txt"); }
