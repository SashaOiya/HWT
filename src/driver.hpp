#pragma once 

#include <iostream>
#include <set>
#include <vector>

#include "hwt.hpp"

namespace {

using KeyT = int;

template <typename Tree_t>
std::size_t range_query(const Tree_t& tree, const KeyT fst, const KeyT snd) {
    if (fst >= snd) {
        return 0;
    }
    if constexpr (std::is_same_v<Tree_t, avl_tree::SearchTree<KeyT>>)
        return tree.my_distance(fst, snd);
    else
        return std::distance(tree.lower_bound(fst), tree.upper_bound(snd));
}

template <typename Tree_t>
void get_answer(std::vector<KeyT> &answer) {
    Tree_t tree = {};

    char type = 0;
    while (std::cin >> type) {
        if (!std::cin.good()) {
            throw std::runtime_error("Invalid type of variable");
        }
        if (type == 'k') {
            KeyT key;
            std::cin >> key;
            if (!std::cin.good()) {
                throw std::runtime_error("Invalid key");
            }

            tree.insert(key);
        } else if (type == 'q') {
            KeyT key1, key2;

            std::cin >> key1;
            if (!std::cin.good()) {
                throw std::runtime_error("Invalid key1");
            }
            std::cin >> key2;
            if (!std::cin.good()) {
                throw std::runtime_error("Invalid key2");
            }

            answer.push_back(range_query<Tree_t>(tree, key1, key2));
        } else {
            throw std::invalid_argument("Invalid type");
        }
    }

#if defined(AVL_TREE) && (DUMP)
    tree.graph_dump("../../tree.dot");
#endif
}

}