#pragma once

#include <cstddef>
#include <iterator>
#include <utility>

#include "node.hpp"

namespace avl_tree {

template <typename KeyT>
class tree_iterator final {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = KeyT;
    using pointer = const KeyT*;
    using reference = const KeyT&;
    using difference_type = std::ptrdiff_t;

    explicit tree_iterator(const TreeNode<KeyT>* node = nullptr) noexcept : current(node) {}

    bool operator==(const tree_iterator& other) const { return current == other.current; }
    auto& operator*() const { return current->key_; }

    tree_iterator& operator++() {
        current = current ? current->next_ : nullptr;
        return *this;
    }

    tree_iterator operator++(int) {
        tree_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    tree_iterator& operator--() {
        current = current ? current->prev_ : nullptr;
        return *this;
    }

    tree_iterator operator--(int) {
        tree_iterator tmp = *this;
        --(*this);
        return tmp;
    }

   private:
    const TreeNode<KeyT>* current;

    void advance_part() noexcept {
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