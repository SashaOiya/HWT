#pragma once 

#include <iostream>
#include <iterator>
#include <utility>

namespace hwt {
// final
template <typename KeyT>
struct Node_s { // smart ptr
    KeyT key_;
    Node_s *parent_ = nullptr, *left_ = nullptr, *right_ = nullptr;
    size_t height_ = 0;
    Node_s ( const KeyT& val, Node_s* parent = nullptr ) : key_(val), parent_(parent) {}
};

template <typename KeyT>
class iterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = KeyT;
    using difference_type = std::ptrdiff_t;
    using pointer = KeyT*;
    using reference = KeyT&;

    explicit iterator ( const Node_s<KeyT>* node = nullptr ) : current(node) {}

    const KeyT& operator*() const {
        return current->key_;
    }

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

    const bool operator==(const iterator& other) const {
        return current == other.current;
    }

    bool operator!=(const iterator& other) const { // delete
        return current != other.current;
    }

private:
    const Node_s<KeyT>* current;

    // Метод для перехода к следующему узлу в порядке in-order
    void advanceForward () // new method
    {
        if ( current->right_ ) {
            current = current->right_;
            while ( current->left_ ) {
                current = current->left_;
            }
        } else {
            auto parent = current->parent_;
            while ( parent && current == parent->right_ ) {
                current = std::exchange ( parent, parent->parent_ );
            }
            current = parent;
        }
    }

    // Метод для перехода к предыдущему узлу в порядке in-order
    void advanceBackward () 
    {
        if ( current->left_ ) {
            current = current->left_;
            while ( current->right_ ) {
                current = current->right_;
            }
        } else {
            auto parent = current->parent_;
            while ( parent && current == parent->left_ ) {
                current = std::exchange ( parent, parent->parent_ );
            }
            current = parent;
        }
    }
};

}