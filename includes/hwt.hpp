#pragma once

#include <fstream>
#include <iostream>
#include <stack>
#include <utility>
#include <vector>

#include "node_iterator.hpp"

namespace my_tree {

template <typename KeyT>
class SearchTree final {
    using Node = my_tree::Node<KeyT>;
    Node *top_ = nullptr;
    std::vector<Node *> nodes_{};
    Node *min_key_node_ = nullptr;

   public:
    using iterator = my_tree::iterator<KeyT>;
    using const_iterator = iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = reverse_iterator;

    SearchTree(std::initializer_list<KeyT> l) {
        nodes_.reserve(l.size());

        for (auto &itt : l) {
            insert(itt);
        }
    }

    SearchTree(const SearchTree &tree) {
        nodes_.reserve(tree.size());

        for (auto i : tree.nodes_) {
            insert(i->key_);
        }
    }

    SearchTree &operator=(const SearchTree &tree) {
        SearchTree copy{tree};
        swap(copy);

        return *this;
    }

    SearchTree(SearchTree &&rhs) noexcept
        : top_(std::exchange(rhs.top_, nullptr)),
          min_key_node_(std::exchange(rhs.min_key_node_, nullptr)),
          nodes_(std::move(rhs.nodes_)) {}

    SearchTree operator=(SearchTree &&rhs) noexcept {
        swap(rhs);
        return *this;
    }

    ~SearchTree() {
        for (auto iter : nodes_) {
            delete (iter);
        }
    }

    size_t size() const noexcept { return nodes_.size(); }
    bool empty() const noexcept { return !top_; }

    const_iterator begin() const noexcept { return const_iterator(min_key_node_); }
    const_iterator end() const noexcept { return const_iterator(nullptr); }

    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator{end()}; }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator{begin()}; }

    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend() const noexcept { return end(); }

    const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    const_reverse_iterator crend() const noexcept { return rend(); }

    void insert(const KeyT &key) {
        if (!top_) {
            top_ = new Node(key, top_);
            nodes_.push_back(top_);
            min_key_node_ = top_;

            return;
        }

        Node *current = top_;
        Node *parent = nullptr;

        while (current) {
            parent = current;
            if (key < current->key_) {
                current = current->left_;
            } else if (current->key_ < key) {
                current = current->right_;
            } else {
                return;
            }
        }

        current = new Node(key, parent);
        if (key < parent->key_) {
            parent->left_ = current;
        } else {
            parent->right_ = current;
        }

        nodes_.push_back(current);
        if (key < min_key_node_->key_) {
            min_key_node_ = current;
        }

        Node *node = current;

        while (node) {
            node = balance(node);
            node = node->parent_;
        }

        return;
    }

    void swap(SearchTree &tree) noexcept {
        std::swap(top_, tree.top_);
        std::swap(nodes_, tree.nodes_);
        std::swap(min_key_node_, tree.min_key_node_);
    }

    auto lower_bound(const KeyT &value) const {
        Node *node = top_;
        Node *result = nullptr;

        while (node) {
            if (value <= node->key_) {
                result = std::exchange(node, node->left_);
            } else {
                node = node->right_;
            }
        }

        return iterator{result};
    }

    auto upper_bound(const KeyT &value) const {
        Node *node = top_;
        Node *result = nullptr;
        while (node) {
            if (value < node->key_) {
                result = std::exchange(node, node->left_);
            } else {
                node = node->right_;
            }
        }

        return iterator{result};
    }

    int getRank(Node *node, const KeyT &val) const {
        int rank = 0;
        while (node) {
            if (val < node->key_) {
                node = node->left_;
            } else {
                rank += Node::get_size(node->left_) + 1;
                if (val == node->key_) break;
                node = node->right_;
            }
        }
        return rank;
    }

    int my_distance(const KeyT &x, const KeyT &y) const {
        return getRank(top_, y) - getRank(top_, x - 1);
    }

   private:
    Node *balance(Node *p) {
        Node::fix_height(p);

        const auto b_factor_value = Node::b_factor(p);
        if (b_factor_value == 2) {
            if (Node::b_factor(p->right_) < 0) {
                p->right_ = rotate_right(p->right_);
            }
            return rotate_left(p);
        }
        if (b_factor_value == -2) {
            if (Node::b_factor(p->left_) > 0) {
                p->left_ = rotate_left(p->left_);
            }
            return rotate_right(p);
        }

        return p;
    }

    Node *rotate_left(Node *q) {
        auto p = q->right_;
        q->right_ = p->left_;
        if (p->left_) p->left_->parent_ = q;
        p->left_ = q;

        p->parent_ = q->parent_;
        if (q->parent_) {
            if (q->parent_->left_ == q)
                q->parent_->left_ = p;
            else
                q->parent_->right_ = p;
        } else {
            top_ = p;
        }

        q->parent_ = p;

        Node::fix_height(q);
        Node::fix_height(p);

        return p;
    }

    Node *rotate_right(Node *p) {
        auto q = p->left_;
        p->left_ = q->right_;
        if (q->right_) q->right_->parent_ = p;
        q->right_ = p;

        q->parent_ = p->parent_;
        if (p->parent_) {
            if (p->parent_->left_ == p)
                p->parent_->left_ = q;
            else
                p->parent_->right_ = q;
        } else {
            top_ = q;
        }
        p->parent_ = q;

        Node::fix_height(p);
        Node::fix_height(q);

        return q;
    }

    void graph_dump_nodes(std::ofstream &file, Node *root) const {
        if (!root) return;

        std::stack<Node *> stack = {};
        stack.push(root);

        bool is_root = true;
        while (!stack.empty()) {
            auto node = stack.top();

            file << "\"" << node << "\""
                 << "[shape = Mrecord, style = filled, fillcolor = lightpink "
                    "label = \""
                 << node->key_ << "\"]" << std::endl;

            if (!is_root)
                file << "\"" << node->parent_ << "\"" << " -> " << "\"" << node << "\""
                     << std::endl;
            else
                is_root = false;

            stack.pop();

            if (node->right_) {
                stack.push(node->right_);
            }
            if (node->left_) {
                stack.push(node->left_);
            }
        }
    }

   public:
    void graph_dump(std::string filename) const {
        std::ofstream file(filename);
        file << "digraph G {" << std::endl << "node [shape = record];" << std::endl;

        if (top_) graph_dump_nodes(file, top_);

        file << "}";
        file.close();

        std::string command = "dot -T png " + filename + " -o ../../logs/tree5.png";
        std::system(command.c_str());
    }

    void text_dump(const Node *tree_node) {
        if (tree_node == nullptr) {
            printf(" nil ");

            return;
        }
        printf(" ( ");
        printf("%d", tree_node->key_);

        Tree_Text_Dump(tree_node->left_);
        Tree_Text_Dump(tree_node->right_);

        printf(" ) ");
    }
};

}  // namespace my_tree
