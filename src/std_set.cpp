#include <iostream>
#include <set>

int main() try {
    using KeyT = int;
    std::set<KeyT> tree = {};

    char type = 0;
    while (std::cin >> type) {
        if (type == 'k') {
            KeyT key = 0;
            std::cin >> key;
            if (!std::cin.good()) {
                throw std::runtime_error("Invalid key");
            }

            tree.insert(key);
        } else if (type == 'q') {
            KeyT key1 = 0;
            KeyT key2 = 0;

            std::cin >> key1;
            if (!std::cin.good()) {
                throw std::runtime_error("Invalid key1");
            }
            std::cin >> key2;
            if (!std::cin.good()) {
                throw std::runtime_error("Invalid key2");
            }
            if (key1 >= key2) {
                std::cout << 0 << ' ';
                continue;
            }

            std::cout << std::distance(tree.lower_bound(key1), tree.upper_bound(key2)) << ' ';
        } else if (type) {
            throw std::invalid_argument("Invalid type");
        }
    }

    return 0;
} catch (const std::exception& e) {
    std::cerr << "Error : " << e.what() << '\n';
    return EXIT_FAILURE;
} catch (...) {
    std::cerr << "Caught unknown exception\n";
    return EXIT_FAILURE;
}