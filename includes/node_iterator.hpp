#pragma once

namespace my_tree {
template <typename KeyT>
struct Node final {
    KeyT key_;
    Node *parent_ = nullptr, *left_ = nullptr, *right_ = nullptr;
    std::size_t height_ = 0, size_ = 1;
    Node(const KeyT& val, Node* parent = nullptr) : key_(val), parent_(parent) {}

    static std::size_t get_height(const Node* node) { return node ? node->height_ : 0; }

    static std::size_t get_size(const Node* node) { return node ? node->size_ : 0; }
    static std::make_signed_t<std::size_t> b_factor(const Node* node) {
        return get_height(node->right_) - get_height(node->left_);
    }

    static void fix_height(Node* p) {  // and size
        p->height_ = 1 + std::max(get_height(p->left_), get_height(p->right_));
        p->size_ = get_size(p->left_) + get_size(p->right_) + 1;
    }
};

template <typename KeyT>
class iterator final {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = KeyT;
    using difference_type = std::ptrdiff_t;
    using pointer = const KeyT*;
    using reference = const KeyT&;

    explicit iterator(const Node<KeyT>* node = nullptr) noexcept : current(node) {}

    bool operator==(const iterator& other) const { return current == other.current; }
    auto& operator*() const { return current->key_; }

    iterator& operator++() {
        advanceForward();
        return *this;
    }

    iterator operator++(int) {
        auto temp = *this;
        advanceForward();
        return temp;
    }

    iterator& operator--() {
        advanceBackward();
        return *this;
    }

    iterator operator--(int) {
        auto temp = *this;
        advanceBackward();
        return temp;
    }

   private:
    const Node<KeyT>* current;

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

}  // namespace my_tree