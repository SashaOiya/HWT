#include <fstream>
#include <string>

#include <gtest/gtest.h>

#include "hwt.hpp"
#include "funcs.hpp"

TEST ( test1, test1_unit )
{
	test_funcs::copy_ctor_test ( "/test1.txt" );
}

TEST ( test2, test2_unit )
{
	test_funcs::copy_assignment_test ( "/test2.txt" );
}

TEST ( test3, test3_unit )
{
	test_funcs::move_ctor_test ( "/test3.txt" );
}

TEST ( test4, test4_unit )
{
	test_funcs::move_assignment_test ( "/test4.txt" );
}
