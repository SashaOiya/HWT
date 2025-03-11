#include <benchmark/benchmark.h>

#include <set>

#include "hwt.hpp"

using KeyT = int;

template <typename Tree_t>
std::size_t range_query(const Tree_t& tree, const KeyT fst, const KeyT snd) {
    if (fst >= snd) {
        return 0;
    }
    if constexpr (std::is_same_v<Tree_t, my_tree::SearchTree<KeyT>>)
        return tree.my_distance(fst, snd);
    else
        return std::distance(tree.lower_bound(fst), tree.upper_bound(snd));
}

template <typename Tree_t>
void get_answer(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Invalid filename");
    }

    Tree_t tree = {};
    std::vector<int> res = {};

    char type = 0;
    while (file >> type) {
        if (!file.good()) {
            throw std::runtime_error("Invalid type of variable");
        }
        if (type == 'k') {
            KeyT key;
            file >> key;
            if (!file.good()) {
                throw std::runtime_error("Invalid key");
            }

            tree.insert(key);
        } else if (type == 'q') {
            KeyT key1, key2;

            file >> key1;
            if (!file.good()) {
                throw std::runtime_error("Invalid key1");
            }
            file >> key2;
            if (!file.good()) {
                throw std::runtime_error("Invalid key2");
            }

            res.push_back(range_query<Tree_t>(tree, key1, key2));
        } else {
            throw std::invalid_argument("Invalid type");
        }
    }
}

static void BM_MyTree(benchmark::State& state) {
    for (auto _ : state) {
        const std::string path = "../benchmark/data/test2.txt";
        get_answer<my_tree::SearchTree<KeyT>>(path);
    }
}

static void BM_StdSet(benchmark::State& state) {
    for (auto _ : state) {
        const std::string path = "../benchmark/data/test2.txt";
        get_answer<std::set<KeyT>>(path);
    }
}

BENCHMARK(BM_MyTree)->Iterations(1);
BENCHMARK(BM_StdSet)->Iterations(1);

BENCHMARK_MAIN();