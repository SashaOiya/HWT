#include <benchmark/benchmark.h>

#include <set>

#include "driver.hpp"
#include "hwt.hpp"

static void BM_MyTree(benchmark::State& state) {
    std::ifstream file("../benchmark/data/test2.txt");
    if (!file) {
        throw std::runtime_error("Reading file error");
    }
    std::cin.rdbuf(file.rdbuf());

    std::vector<KeyT> answer = {};
    for (auto _ : state) {
        get_answer<my_tree::SearchTree<KeyT>>(answer);
    }
}

static void BM_StdSet(benchmark::State& state) {
    std::ifstream file("../benchmark/data/test2.txt");
    if (!file) {
        throw std::runtime_error("Reading file error");
    }
    std::cin.rdbuf(file.rdbuf());

    std::vector<KeyT> answer = {};
    for (auto _ : state) {
        get_answer<std::set<KeyT>>(answer);
    }
}

BENCHMARK(BM_MyTree)->Iterations(1);
BENCHMARK(BM_StdSet)->Iterations(1);

BENCHMARK_MAIN();