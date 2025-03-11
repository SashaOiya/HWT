#include <cassert>

#include "hwt.hpp"

using KeyT = int;

auto range_query(my_tree::SearchTree<KeyT, KeyT>& tree, const KeyT fst, const KeyT snd) {
    if (fst >= snd) {
        return 0;
    }

    return tree.my_distance(fst, snd);
}

int main() try {
    my_tree::SearchTree<KeyT, KeyT> tree = {};

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

            std::cout << range_query(tree, key1, key2) << ' ';
        } else {
            throw std::invalid_argument("Invalid type");
        }
    }

#ifdef DUMP
    tree.graph_dump("../../tree.dot");
#endif

    return 0;
} catch (const std::exception& e) {
    std::cerr << "Error : " << e.what() << '\n';
    return EXIT_FAILURE;
} catch (...) {
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
}