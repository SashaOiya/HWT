#include "includes/hwt.hpp"
#include <fstream>
#include <set>

int main ()
{
    using KeyT = int;
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

            std:: cout << tree.range_query ( key1, key2 ) << ' ';
        } 
        else if ( type ) { 
            std::cout << "Error: invalit type\n";
            return EXIT_FAILURE;
        } 
    }
    std::cout << std::endl;

    /*std::set<int> tree = {}; 
    tree.insert(9);
    tree.insert(4);
    tree.insert(7);
    tree.insert(8);*/

    tree.graph_dump( );

    return 0;
}