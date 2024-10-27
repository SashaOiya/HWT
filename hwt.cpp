#include "hwt.hpp"
#include <set>

int main ()
{
    Trees::SearchTree<int, int> tree = {};

    tree.read_data ();
    /*std::set<int> tree = {}; 
    tree.insert(9);
    tree.insert(4);
    tree.insert(7);
    tree.insert(8);*/

    tree.Tree_Graph_Dump( );

    std::cout << "LOX\n";

    for (auto it = tree.begin(); it != tree.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;


    return 0;
}