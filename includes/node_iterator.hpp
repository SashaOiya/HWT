#pragma once

#include <utility>
#include <iterator>
#include <cstddef>
#include "node.hpp"

namespace avl_tree {

template <typename KeyT>
class tree_iterator final {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = KeyT;
    using pointer    = const KeyT*;
    using reference  = const KeyT&;
    using difference_type = std::ptrdiff_t;

    explicit tree_iterator(const TreeNode<KeyT>* node = nullptr) noexcept : current(node) {}

    bool operator==(const tree_iterator& other) const { return current == other.current; }
    auto& operator*() const { return current->key_; }

    tree_iterator& operator++() {
        advanceForward();
        return *this;
    }

    tree_iterator operator++(int) {
        auto temp = *this;
        advanceForward();
        return temp;
    }

    tree_iterator& operator--() {
        advanceBackward();
        return *this;
    }

    tree_iterator operator--(int) {
        auto temp = *this;
        advanceBackward();
        return temp;
    }

   private:
    const TreeNode<KeyT>* current;

    void advance_part( ) noexcept {
        if (current->right_) {
            current = current->right_;
            while (current->left_) {
                current = current->left_;
            }
        } else {
            auto parent = current->parent_;
            while (parent && current == parent->right_) {
                current = std::exchange(parent, parent->parent_);
            }
            current = parent;
        }
    }

    void advanceForward() noexcept {
        if (current->right_) {
            current = current->right_;
            while (current->left_) {
                current = current->left_;
            }
        } else {
            auto parent = current->parent_;
            while (parent && current == parent->right_) {
                current = std::exchange(parent, parent->parent_);
            }
            current = parent;
        }
    }

    void advanceBackward() noexcept {
        if (current->left_) {
            current = current->left_;
            while (current->right_) {
                current = current->right_;
            }
        } else {
            auto parent = current->parent_;
            while (parent && current == parent->left_) {
                current = std::exchange(parent, parent->parent_);
            }
            current = parent;
        }
    }
};

}  // namespace avl_tree