#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <gtest/gtest.h>

#include "hwt.hpp"

namespace test_funcs
{
    void get_data (const std::string& filename, Trees::SearchTree<int, int> &tree)
	{
        std::ifstream file ( filename );
        if (!file) {
            std::cerr << "error\n";
            exit(1);
        }

        char type = 0;
        while ( file >> type ) {
            if ( type == 'k' ) {
                int key = 0;
                file >> key;
                if ( !file.good() ) { throw "Error : invalid type of key\n"; }

                tree.insert ( key );
            }
            else if ( type == 'q' ) {
                int key1 = 0;
                int key2 = 0;

                file >> key1;
                if ( !std::cin.good() ) { throw "Error : invalid type of key1\n"; }

                file >> key2;
                if ( !std::cin.good() ) { throw "Error : invalid type of key2\n"; }
            }
            else if ( type ) { throw "Error: invalit type\n"; }
        }
    }

	void copy_ctor_test (const std::string& test_name)
	{
        std::string test_directory = "/tests";
		std::string test_path = std::string ( TEST_DATA_DIR) + test_directory + test_name;

        Trees::SearchTree<int, int> tree = {};

        get_data ( test_path, tree );

        Trees::SearchTree<int, int> lhs = tree;

        EXPECT_TRUE(tree.size() == lhs.size());
        for ( auto itt = tree.begin(), itt_lhs = lhs.begin(), end = tree.end() ; itt != end; ++itt, ++itt_lhs ) {
            EXPECT_EQ ( *itt , *itt_lhs );
        }
	}

    void copy_assignment_test ( const std::string& test_name )
	{
        std::string test_directory = "/tests";
		std::string test_path = std::string(TEST_DATA_DIR) + test_directory + test_name;

        Trees::SearchTree<int, int> tree = {};
        get_data ( test_path, tree );

        std::vector<int> data = {};
        for ( auto itt = tree.begin(), end = tree.end() ; itt != end; ++itt ) {
            data.push_back ( *itt );
        }

        Trees::SearchTree<int, int> lhs = {};
        lhs = tree;

        EXPECT_TRUE(data.size() == lhs.size());
        auto itt_lhs = lhs.begin();
        for ( auto itt = data.begin(), end = data.end() ; itt != end; ++itt, ++itt_lhs ) {
            EXPECT_EQ ( *itt , *itt_lhs );
        }
	}

    void move_ctor_test ( const std::string& test_name )
	{
        std::string test_directory = "/tests";
		std::string test_path = std::string(TEST_DATA_DIR) + test_directory + test_name;

        Trees::SearchTree<int, int> tree = {};
        get_data ( test_path, tree );

        std::vector<int> data = {};
        for ( auto itt = tree.begin(), end = tree.end() ; itt != end; ++itt ) {
            data.push_back ( *itt );
        }

        Trees::SearchTree<int, int> lhs = std::move ( tree );

        EXPECT_TRUE(data.size() == lhs.size());
        auto itt_lhs = lhs.begin();
        for ( auto itt = data.begin(), end = data.end() ; itt != end; ++itt, ++itt_lhs ) {
            EXPECT_EQ ( *itt , *itt_lhs );
        }
	}

    void move_assignment_test ( const std::string& test_name )
	{
        std::string test_directory = "/tests";
		std::string test_path = std::string(TEST_DATA_DIR) + test_directory + test_name;

        Trees::SearchTree<int, int> tree = {};
        get_data ( test_path, tree );

        std::vector<int> data = {};
        for ( auto itt = tree.begin(), end = tree.end() ; itt != end; ++itt ) {
            data.push_back ( *itt );
        }

        Trees::SearchTree<int, int> lhs = {};
        lhs = std::move ( tree );

        EXPECT_TRUE(data.size() == lhs.size());
        auto itt_lhs = lhs.begin();
        for ( auto itt = data.begin(), end = data.end() ; itt != end; ++itt, ++itt_lhs ) {
            EXPECT_EQ ( *itt , *itt_lhs );
        }
	}
}