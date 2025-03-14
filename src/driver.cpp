//#include <CLI/CLI.hpp>
#include "driver.hpp"

int main() try {
#if defined(AVL_TREE)
    using tree_type = my_tree::SearchTree<KeyT>;
#else
    using tree_type = std::set<KeyT>;
#endif
    std::vector<KeyT> answer = {};
    get_answer<tree_type>(answer);

    for (auto& it : answer) std::cout << it << ' ';

    return 0;
} catch (const std::exception& e) {
    std::cerr << "Error : " << e.what() << '\n';
    return EXIT_FAILURE;
} catch (...) {
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
}