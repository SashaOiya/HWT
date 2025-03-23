#include "driver.hpp"

#include <CLI/CLI.hpp>

int main(int argc, char** argv) try {
    CLI::App app{"HWT"};

    std::string dot_path, output_file;
    app.add_option("--dot", dot_path, "Path to the .dot file with the tree");
    app.add_option("--output", output_file, "Path to the .out file with the tree graph");
    CLI11_PARSE(app, argc, argv);

#if defined(AVL_TREE)
    using tree_type = avl_tree::SearchTree<KeyT>;
#else
    using tree_type = std::set<KeyT>;
#endif

    for (auto ans : get_answer<tree_type>(dot_path, output_file)) std::cout << ans << ' ';

    return 0;
} catch (const std::exception& e) {
    std::cerr << "Error : " << e.what() << '\n';
    return EXIT_FAILURE;
} catch (...) {
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
}
