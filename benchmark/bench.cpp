#include <benchmark/benchmark.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "hwt.hpp"

using KeyT = int;

auto range_query(my_tree::SearchTree<KeyT, KeyT>& tree, const KeyT& fst, const KeyT& snd) {
    return tree.my_distance(fst, snd);
}

void get_result(const std::string& filename) {
    std::vector<int> res = {};
    std::ifstream file(filename);
    if (!file) {
        std::cout << "Invalid filename : " << filename << '\n';
        exit(1);
    }

    my_tree::SearchTree<KeyT, KeyT> tree = {};

    char type = 0;
    while (file >> type) {
        if (type == 'k') {
            KeyT key = 0;
            file >> key;
            if (!file.good()) {
                std::cout << "Error : invalid type of key\n";
                return;
            }

            tree.insert(key);
        } else if (type == 'q') {
            KeyT key1 = 0;
            KeyT key2 = 0;

            file >> key1;
            if (!std::cin.good()) {
                std::cout << "Error : invalid type of key1\n";
                return;
            }

            file >> key2;
            if (!std::cin.good()) {
                std::cout << "Error : invalid type of key2\n";
                return;
            }

            if (key1 >= key2) {
                res.push_back(0);
                continue;
            }

            res.push_back(range_query(tree, key1, key2));
        } else if (type) {
            std::cout << "Error: invalit type\n";
            return;
        }
    }
}

void get_answer(const std::string& filename) {
    std::vector<KeyT> ans = {};
    std::ifstream file(filename);
    if (!file) {
        std::cout << "error\n";
        exit(1);
    }

    using KeyT = int;
    std::set<KeyT> tree = {};

    char type = 0;
    while (file >> type) {
        if (type == 'k') {
            KeyT key = 0;
            file >> key;
            if (!file.good()) {
                throw "Error : invalid type of key\n";
            }

            tree.insert(key);
        } else if (type == 'q') {
            KeyT key1 = 0;
            KeyT key2 = 0;

            file >> key1;
            if (!std::cin.good()) {
                throw "Error : invalid type of key1\n";
            }

            file >> key2;
            if (!std::cin.good()) {
                throw "Error : invalid type of key2\n";
            }

            if (key1 >= key2) {
                ans.push_back(0);
                continue;
            }

            ans.push_back(std::distance(tree.lower_bound(key1), tree.upper_bound(key2)));
        } else if (type) {
            throw "Error: invalit type\n";
        }
    }
}

static void BM_MyTree(benchmark::State& state) {
    for (auto _ : state) {
        const std::string path = "../benchmark/data/test2.txt";
        get_result(path);
    }
}

static void BM_StdSet(benchmark::State& state) {
    for (auto _ : state) {
        const std::string path = "../benchmark/data/test2.txt";
        get_answer(path);
    }
}

BENCHMARK(BM_MyTree)->Iterations(1);
BENCHMARK(BM_StdSet)->Iterations(1);

BENCHMARK_MAIN();