#include <gtest/gtest.h>

#include <vector>

#include "hwt.hpp"

TEST(my_AVL_tree, copy_ctor) {
    my_tree::SearchTree<int, int> tree = {1, 2, 3, 4, 6, 7, 8, 12, 1};
    my_tree::SearchTree<int, int> lhs = tree;

    EXPECT_TRUE(tree.size() == lhs.size());
    for (auto itt = tree.begin(), itt_lhs = lhs.begin(), end = tree.end(); itt != end;
         ++itt, ++itt_lhs) {
        EXPECT_EQ(*itt, *itt_lhs);
    }
}

TEST(my_AVL_tree, copy_assignment) {
    my_tree::SearchTree<int, int> tree = {3, 6, 9, 1, 5, 10, 22};

    std::vector<int> data = {};
    for (auto itt : tree) {
        data.push_back(itt);
    }

    my_tree::SearchTree<int, int> lhs = {};
    lhs = tree;

    EXPECT_TRUE(data.size() == lhs.size());
    auto itt_lhs = lhs.begin();
    for (auto itt = data.begin(), end = data.end(); itt != end; ++itt, ++itt_lhs) {
        EXPECT_EQ(*itt, *itt_lhs);
    }
}

TEST(my_AVL_tree, move_ctor) {
    my_tree::SearchTree<int, int> tree = {-4, 9, 6, 4, -11, 0, 7, 4};

    std::vector<int> data = {};
    for (auto itt : tree) {
        data.push_back(itt);
    }

    my_tree::SearchTree<int, int> lhs = std::move(tree);

    EXPECT_TRUE(data.size() == lhs.size());
    auto itt_lhs = lhs.begin();
    for (auto itt = data.begin(), end = data.end(); itt != end; ++itt, ++itt_lhs) {
        EXPECT_EQ(*itt, *itt_lhs);
    }
}

TEST(my_AVL_tree, move_assignment) {
    my_tree::SearchTree<int, int> tree = {5, 8, 9, 12, 3, -4, 67, 18};

    std::vector<int> data = {};
    for (auto itt : tree) {
        data.push_back(itt);
    }

    my_tree::SearchTree<int, int> lhs = {};
    lhs = std::move(tree);

    EXPECT_TRUE(data.size() == lhs.size());
    auto itt_lhs = lhs.begin();
    for (auto itt = data.begin(), end = data.end(); itt != end; ++itt, ++itt_lhs) {
        EXPECT_EQ(*itt, *itt_lhs);
    }
}
