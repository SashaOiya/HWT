#include "includes/hwt.hpp"

#include <cassert>
#include <chrono>

using KeyT = int;

auto range_query(my_tree::SearchTree<KeyT, KeyT>& tree, const KeyT& fst, const KeyT& snd) {
    if (fst >= snd) {
        return 0;
    }

    return tree.my_distance(fst, snd);
}

int main() {
    my_tree::SearchTree<KeyT, KeyT> tree = {};
    std::chrono::duration<double> total_time{0};

    char type = 0;
    while (std::cin >> type) {
        if (type == 'k') {
            KeyT key = 0;
            std::cin >> key;
            if (!std::cin.good()) {
                std::cout << "Error : invalid type of key\n";
                return EXIT_FAILURE;
            }

            tree.insert(key);
        } else if (type == 'q') {
            KeyT key1 = 0;
            KeyT key2 = 0;

            std::cin >> key1;
            if (!std::cin.good()) {
                std::cout << "Error : invalid type of key1\n";
                return EXIT_FAILURE;
            }
            std::cin >> key2;
            if (!std::cin.good()) {
                std::cout << "Error : invalid type of key2\n";
                return EXIT_FAILURE;
            }

            auto begin_time_of_range_query = std::chrono::high_resolution_clock::now();
            std::cout << range_query(tree, key1, key2) << ' ';
            auto end_time_of_range_query = std::chrono::high_resolution_clock::now();
            total_time += end_time_of_range_query - begin_time_of_range_query;
        } else if (type) {
            std::cout << "Error: invalit type\n";
            return EXIT_FAILURE;
        }
    }
    std::cout << "Total time: "
              << std::chrono::duration_cast<std::chrono::microseconds>(total_time).count();
    std::cerr << " in microseconds" << std::endl;

#ifdef DUMP
    tree.graph_dump("../tree.dot");
#endif

    return 0;
}