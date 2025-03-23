#pragma once

#include <algorithm>
#include <cstdio>
#include <type_traits>
#include <cassert>

namespace avl_tree {

template <typename KeyT>
class TreeNode final {
    std::size_t height_ = 0, size_ = 1;

   public:
    KeyT key_;
    TreeNode *parent_, *left_ = nullptr, *right_ = nullptr;

    TreeNode(const KeyT& val, TreeNode* parent = nullptr) : key_(val), parent_(parent) {}

    static std::size_t get_height(const TreeNode* node) noexcept {
        return node ? node->height_ : 0;
    }

    static std::size_t get_size(const TreeNode* node) noexcept { return node ? node->size_ : 0; }
    static std::make_signed_t<std::size_t> b_factor(const TreeNode* node) noexcept {
        assert(node);
        return get_height(node->right_) - get_height(node->left_);
    }

    void fix_height() {
        height_ = 1 + std::max(get_height(left_), get_height(right_));
        size_ = get_size(left_) + get_size(right_) + 1;
    }
};
}  // namespace avl_tree