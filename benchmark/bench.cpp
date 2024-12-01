#include <benchmark/benchmark.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "../includes/hwt.hpp"

using KeyT = int;

auto range_query ( const Trees::SearchTree<KeyT, KeyT> &tree, const KeyT& fst, const KeyT& snd )
{
    auto start = tree.lower_bound ( fst );
    auto fin = tree.upper_bound ( snd );

    return std::distance ( start, fin );
}

void get_result ( const std::string& filename )
{
    std::vector<int> res = {};
    std::ifstream file ( filename );
    if (!file) {
        std::cout << "Invalid filename : " << filename << '\n' ;
        exit(1);
    }

    Trees::SearchTree<KeyT, KeyT> tree = {};

    char type = 0;
    while ( file >> type ) {
        if ( type == 'k' ) {
            KeyT key = 0;
            file >> key;
            if ( !file.good() ) { std::cout << "Error : invalid type of key\n"; return ; }

            tree.insert ( key );
        }
        else if ( type == 'q' ) {
            KeyT key1 = 0;
            KeyT key2 = 0;

            file >> key1;
            if ( !std::cin.good() ) { std::cout << "Error : invalid type of key1\n"; return ; }

            file >> key2;
            if ( !std::cin.good() ) { std::cout << "Error : invalid type of key2\n"; return ; }

            if ( key1 >= key2 ) { res.push_back( 0 ); continue; }

            res.push_back ( range_query ( tree, key1, key2 ) );
        }
        else if ( type ) { std::cout << "Error: invalit type\n"; return ; }
    }
}

static void BM_StringCreation(benchmark::State& state) {
    for (auto _ : state) {
        const std::string path = "../tests/test2.txt";
        get_result ( path );
    }
}

BENCHMARK(BM_StringCreation)->Iterations( 1 );

BENCHMARK_MAIN();