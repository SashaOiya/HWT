#include "includes/hwt.hpp"
#include <fstream>

using KeyT = int;

auto range_query ( const Trees::SearchTree<KeyT, KeyT> &tree, const KeyT& fst, const KeyT& snd )
{
    auto start = tree.lower_bound ( fst );
    auto fin = tree.upper_bound ( snd );

    return std::distance ( start, fin );
}

int main ()
{
    Trees::SearchTree<KeyT, KeyT> tree = {};

    char type = 0;
    while ( std::cin >> type ) {
        if ( type == 'k' ) {
            KeyT key = 0;
            std::cin >> key;
            if ( !std::cin.good() ) {
                std::cout << "Error : invalid type of key\n";
                return EXIT_FAILURE;
            }

            tree.insert ( key );
        }
        else if ( type == 'q' ) {
            KeyT key1 = 0;
            KeyT key2 = 0;

            std::cin >> key1;
            if ( !std::cin.good() ) {
                std::cout << "Error : invalid type of key1\n";
                return EXIT_FAILURE;
            }
            std::cin >> key2;
            if ( !std::cin.good() ) {
                std::cout << "Error : invalid type of key2\n";
                return EXIT_FAILURE;
            }

            if ( key1 >= key2 ) {
                std::cout << 0 << ' ';
                continue;
            }
            std:: cout << range_query ( tree, key1, key2 ) << ' ';
        }
        else if ( type ) {
            std::cout << "Error: invalit type\n";
            return EXIT_FAILURE;
        }
    }
    std::cout << std::endl;

    //Trees::SearchTree<KeyT, KeyT> copy = tree;
    //tree.graph_dump();

    return 0;
}