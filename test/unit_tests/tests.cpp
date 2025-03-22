#include <gtest/gtest.h>

#include <vector>

#include "hwt.hpp"

TEST(AVL_tree, copy_ctor) {
    avl_tree::SearchTree<int> tree = {1, 2, 3, 4, 6, 7, 8, 12, 1};
    avl_tree::SearchTree<int> lhs = tree;

    EXPECT_TRUE(tree.size() == lhs.size());
    for (auto itt = tree.begin(), itt_lhs = lhs.begin(), end = tree.end(); itt != end;
         ++itt, ++itt_lhs) {
        EXPECT_EQ(*itt, *itt_lhs);
    }
}

TEST(AVL_tree, copy_assignment) {
    avl_tree::SearchTree<int> tree = {3, 6, 9, 1, 5, 10, 22};

    std::vector<int> data = {};
    for (auto itt : tree) {
        data.push_back(itt);
    }

    avl_tree::SearchTree<int> lhs = {};
    lhs = tree;

    EXPECT_TRUE(data.size() == lhs.size());
    auto itt_lhs = lhs.begin();
    for (auto itt = data.begin(), end = data.end(); itt != end; ++itt, ++itt_lhs) {
        EXPECT_EQ(*itt, *itt_lhs);
    }
}

TEST(AVL_tree, move_ctor) {
    avl_tree::SearchTree<int> tree = {-4, 9, 6, 4, -11, 0, 7, 4};

    std::vector<int> data = {};
    for (auto itt : tree) {
        data.push_back(itt);
    }

    avl_tree::SearchTree<int> lhs = std::move(tree);

    EXPECT_TRUE(data.size() == lhs.size());
    auto itt_lhs = lhs.begin();
    for (auto itt = data.begin(), end = data.end(); itt != end; ++itt, ++itt_lhs) {
        EXPECT_EQ(*itt, *itt_lhs);
    }
}

TEST(AVL_tree, move_assignment) {
    avl_tree::SearchTree<int> tree = {5, 8, 9, 12, 3, -4, 67, 18};

    std::vector<int> data = {};
    for (auto itt : tree) {
        data.push_back(itt);
    }

    avl_tree::SearchTree<int> lhs = {};
    lhs = std::move(tree);

    EXPECT_TRUE(data.size() == lhs.size());
    auto itt_lhs = lhs.begin();
    for (auto itt = data.begin(), end = data.end(); itt != end; ++itt, ++itt_lhs) {
        EXPECT_EQ(*itt, *itt_lhs);
    }
}

TEST(AVL_tree, range_based) {
    avl_tree::SearchTree<int> tree = {5, 8, 9, 12, 3, -4, 67, 18};

    std::vector<int> data = {};
    for (auto itt : tree) {
        data.push_back(itt);
    }

    EXPECT_TRUE(data.size() == tree.size());
    auto itt_tree = tree.begin();
    for (auto itt_data = data.begin(), end = data.end(); itt_data != end; ++itt_data, ++itt_tree) {
        EXPECT_EQ(*itt_data, *itt_tree);
    }
}