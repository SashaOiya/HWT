#include <benchmark/benchmark.h>

#include <set>

#include "driver.hpp"
#include "hwt.hpp"

static void BM_MyTree(benchmark::State& state) {
    try {
        std::ifstream file("../benchmark/data/test7.txt");
        if (!file) {
            throw std::runtime_error("Reading file error");
        }
        std::cin.rdbuf(file.rdbuf());

        std::string dot_path, output_file;
        for (auto _ : state) {
            get_answer<avl_tree::SearchTree<KeyT>>(dot_path, output_file);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error : " << e.what() << '\n';
    } catch (...) {
        std::cerr << "Caught unknown exception\n";
    }
}

static void BM_StdSet(benchmark::State& state) {
    try {
        std::ifstream file("../benchmark/data/test7.txt");
        if (!file) {
            throw std::runtime_error("Reading file error");
        }
        std::cin.rdbuf(file.rdbuf());

        std::string dot_path, output_file;
        for (auto _ : state) {
            get_answer<std::set<KeyT>>(dot_path, output_file);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error : " << e.what() << '\n';
    } catch (...) {
        std::cerr << "Caught unknown exception\n";
    }
}

BENCHMARK(BM_MyTree)->Iterations(1);
BENCHMARK(BM_StdSet)->Iterations(1);

BENCHMARK_MAIN();
