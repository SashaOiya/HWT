#pragma once

#include <cstddef>
#include <fstream>
#include <iterator>
#include <memory>
#include <stack>
#include <utility>
#include <vector>

#include "node_iterator.hpp"

namespace avl_tree {

template <typename KeyT>
class SearchTree final {
    using Node = TreeNode<KeyT>;
    Node *top_ = nullptr;
    std::vector<std::unique_ptr<Node>> nodes_;
    Node *min_key_node_ = nullptr;

   public:
    using iterator = tree_iterator<KeyT>;
    using const_iterator = const iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = reverse_iterator;
    using difference_type = std::ptrdiff_t;

    SearchTree(std::initializer_list<KeyT> l) {
        nodes_.reserve(l.size());

        for (auto &itt : l) {
            insert(itt);
        }
    }

    SearchTree(const SearchTree &tree) {
        nodes_.reserve(tree.size());

        for (auto it = tree.nodes_.begin(), ite = tree.nodes_.end(); it != ite; ++it) {
            insert(it->get()->key_);
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

    SearchTree &operator=(SearchTree &&rhs) noexcept {
        swap(rhs);
        return *this;
    }

    ~SearchTree() = default;

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
            auto new_node = std::make_unique<Node>(key, top_);
            top_ = new_node.get();
            min_key_node_ = top_;
            nodes_.push_back(std::move(new_node));

            return;
        }

        auto parent = find_key_parent(key);
        if (!parent) {
            return;
        }

        auto new_node = std::make_unique<Node>(key, parent);
        auto current = new_node.get();
        if (key < parent->key_) {
            parent->left_ = current;
        } else {
            parent->right_ = current;
        }

        nodes_.push_back(std::move(new_node));
        if (key < min_key_node_->key_) {
            min_key_node_ = current;
        }

        while (current) {
            current = balance(current);
            current = current->parent_;
        }
    }

    void swap(SearchTree &tree) noexcept {
        std::swap(top_, tree.top_);
        std::swap(min_key_node_, tree.min_key_node_);
        nodes_.swap(tree.nodes_);
    }

    auto lower_bound(const KeyT &value) const {
        Node *node = top_;
        Node *result = nullptr;

        while (node) {
            if (node->key_ < value) {
                node = node->right_;
            } else {
                result = std::exchange(node, node->left_);
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

    difference_type my_distance(const KeyT &x, const KeyT &y) const {
        return get_rank(y) - get_rank_min(x);
    }

   private:
    Node* find_key_parent(const KeyT &key) const {
        Node *current = top_;
        Node *parent = nullptr;

        while (current) {
            parent = current;
            if (key < current->key_) {
                current = current->left_;
            } else if (current->key_ < key) {
                current = current->right_;
            } else {
                return nullptr;
            }
        }

        return parent;
    }

    difference_type get_rank_min(const KeyT &val) const {
        int rank = 0;
        auto *node = top_;
        while (node) {
            if (val < node->key_) {
                node = node->left_;
            } else {
                const auto left_subtree_size = Node::get_size(node->left_);
                if (node->key_ < val) {
                    rank += left_subtree_size + 1;
                } else {
                    rank += left_subtree_size;
                    break;
                }
                node = node->right_;
            }
        }
        return rank;
    }

    difference_type get_rank(const KeyT &val) const {
        int rank = 0;
        auto *node = top_;
        while (node) {
            if (val < node->key_) {
                node = node->left_;
            } else {
                const auto left_subtree_size = Node::get_size(node->left_);
                if (node->key_ < val) {
                    rank += left_subtree_size + 1;
                } else {
                    rank += left_subtree_size + 1;
                    break;
                }
                node = node->right_;
            }
        }
        return rank;
    }

    Node *balance(Node *p) {
        p->fix_height();

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

        q->fix_height();
        p->fix_height();

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

        p->fix_height();
        q->fix_height();

        return q;
    }

    void graph_dump_nodes(std::ofstream &file, Node *root) const {
        if (!root) return;

        std::stack<Node *> stack = {};  // remove this piece of shit
        stack.push(root);

        bool is_root = true;
        while (!stack.empty()) {
            auto node = stack.top();

            file << "\"" << node << "\""
                 << "[shape = Mrecord, style = filled, fillcolor = lightpink "
                    "label = \""
                 << node->key_ << "\"]" << std::endl;

            if (!is_root)
                file << '\"' << node->parent_ << "\" -> \"" << node << '\"' << std::endl;
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

}  // namespace avl_tree
