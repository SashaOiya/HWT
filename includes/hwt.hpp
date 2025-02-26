#pragma once

#include <fstream>
#include <iostream>
#include <stack>
#include <utility>
#include <vector>

#include "node_iterator.hpp"

namespace Trees {

template <typename KeyT, typename Comp>
class SearchTree {
	using iterator = hwt::iterator<KeyT>;
	using Node_s = hwt::Node_s<KeyT>;
	Node_s *top_ = nullptr;

	Node_s *min_key_node_ = nullptr;
	std::vector<Node_s *> nodes_{};

   public:
	SearchTree() : top_(nullptr) {
	}

	SearchTree(std::initializer_list<KeyT> l) : top_(nullptr) {
		nodes_.reserve(l.size());

		for (auto itt : l) {
			insert(itt);
		}
	}

	SearchTree(const SearchTree &tree) : top_(nullptr) {
		nodes_.reserve(tree.size());

		for (auto &i : tree.nodes_) {
			insert(i->key_);
		}
	}

	SearchTree &operator=(const SearchTree &tree) {
		SearchTree copy{tree};
		swap(copy);

		return *this;
	}

	SearchTree(SearchTree &&rhs) noexcept : nodes_(std::move(rhs.nodes_)) {
		min_key_node_ = std::exchange(rhs.min_key_node_, nullptr);
		top_ = std::exchange(rhs.top_, nullptr);
	}

	SearchTree operator=(SearchTree &&rhs) noexcept {
		swap(rhs);
		return *this;
	}

	~SearchTree() {
		for (auto itt = nodes_.begin(); itt != nodes_.end(); ++itt) {
			delete (*itt);
		}
	}
	size_t size() const {
		return nodes_.size();
	}

	iterator begin() const {
		return iterator(min_key_node_);
	}

	iterator end() const {
		return iterator(nullptr);
	}

	bool empty() const {
		return !top_;
	}

	void insert(const KeyT &value) {
		insert_node(value);
	}

	void swap(SearchTree &tree) noexcept {
		std::swap(top_, tree.top_);
		std::swap(nodes_, tree.nodes_);
		std::swap(min_key_node_, tree.min_key_node_);
	}

	auto lower_bound(const KeyT &value) const {
		Node_s *node = top_;
		Node_s *result = nullptr;

		while (node) {
			if (node->key_ >= value) {
				result = std::exchange(node, node->left_);
			} else {
				node = node->right_;
			}
		}

		return iterator{result};
	}

	auto upper_bound(const KeyT &value) const {
		Node_s *node = top_;
		Node_s *result = nullptr;
		while (node) {
			if (node->key_ > value) {
				result = std::exchange(node, node->left_);
			} else {
				node = node->right_;
			}
		}

		return iterator{result};
	}

	int getRank(Node_s *node, int val) {
		int rank = 0;
		while (node) {
			if (val < node->key_) {
				node = node->left_;
			} else {
				rank += get_size(node->left_) + 1;
				if (val == node->key_) break;
				node = node->right_;
			}
		}
		return rank;
	}

	int my_distance(int x, int y) {
		return getRank(top_, y) - getRank(top_, x - 1);
	}

   private:
	int get_height(const Node_s *p) const {
		return p ? p->height_ : 0;
	}
	int get_size(const Node_s *p) const {
		return p ? p->size_ : 0;
	}

	void insert_node(const KeyT &key) {
		if (!top_) {
			top_ = new Node_s(key, top_);
			nodes_.push_back(top_);
			min_key_node_ = top_;

			return;
		}

		std::stack<Node_s *> path = {};
		Node_s *current = top_;
		Node_s *parent = nullptr;

		while (current) {
			path.push(current);	
			parent = current;
			if (key < current->key_) {
				current = current->left_;
			} else if (key > current->key_) {
				current = current->right_;
			} else {
				return;
			}
		}

		Node_s *new_node = new Node_s(key, path.top());
		if (key < parent->key_) {
			parent->left_ = new_node;
		} else {
			parent->right_ = new_node;
		}
		nodes_.push_back(new_node);
		if (key < min_key_node_->key_) {  
			min_key_node_ = new_node;
		}

		while (!path.empty()) {
			current = path.top();
			path.pop();

			fix_height(current);		
			current = balance(current);	 

			if (path.empty()) {
				top_ = current;
			} else {
				if (current->key_ < path.top()->key_) {
					path.top()->left_ = current;
				} else {
					path.top()->right_ = current;
				}
			}
		}

		return;
	}

	Node_s *balance(Node_s *p) {
		fix_height(p);

		const int b_factor_value = b_factor(p);
		if (b_factor_value == 2) {
			if (b_factor(p->right_) < 0) {
				p->right_ = rotate_right(p->right_);
			}
			return rotate_left(p);
		}
		if (b_factor_value == -2) {
			if (b_factor(p->left_) > 0) {
				p->left_ = rotate_left(p->left_);
			}
			return rotate_right(p);
		}

		return p;
	}

	int b_factor(const Node_s *p) const {
		return get_height(p->right_) - get_height(p->left_);
	}

	void fix_height(Node_s *p) {  // and size
		p->height_ = 1 + std::max(get_height(p->left_), get_height(p->right_));
		p->size_ = get_size(p->left_) + get_size(p->right_) + 1;
	}

	Node_s *rotate_left(Node_s *q) {
		Node_s *p = q->right_;	// copy
		q->right_ = p->left_;
		if (p->left_) p->left_->parent_ = q;
		p->left_ = q;
		p->parent_ = q->parent_;
		q->parent_ = p;

		fix_height(q);
		fix_height(p);

		return p;
	}

	Node_s *rotate_right(Node_s *node) {
		Node_s *new_node = node->left_;
		node->left_ = new_node->right_;
		if (new_node->right_) new_node->right_->parent_ = node;
		new_node->right_ = node;
		new_node->parent_ = node->parent_;
		node->parent_ = new_node;

		fix_height(node);
		fix_height(new_node);

		return new_node;
	}

	void graph_dump_nodes(std::ofstream &file, Node_s *root) const {
		if (!root) return;

		std::stack<Node_s *> stack = {};
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

		std::string command = "dot -T png " + filename + " -o ../logs/tree.png";
		std::system(command.c_str());
	}

	void text_dump(const Node_s *tree_node) {
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

}  // namespace Trees
