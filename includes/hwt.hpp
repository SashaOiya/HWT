#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include "node_iterator.hpp"
#include <stack>
#include <fstream>

namespace Trees {

template <typename KeyT, typename Comp>
class SearchTree { // final
    using iterator = hwt::iterator<KeyT>;
    using Node_s   = hwt::Node_s<KeyT>;
    Node_s *top_ = nullptr;

    Node_s *min_key_node_ = nullptr;
    std::vector <Node_s *> nodes_ {};
public:

    SearchTree () : top_(nullptr) {}

    // auto lhs = rhs;
    // auto lhs{rhs};
    // auto lhs(rhs);
    SearchTree ( const SearchTree &tree ) : top_(nullptr) { // copy ctor
        nodes_.reserve ( tree.size() );

        for ( auto &i : tree.nodes_ ){
            insert ( i->key_ );
        }
    }

    // auto lhs = ....;
    // ....
    // lhs = rhs;
    SearchTree& operator= ( SearchTree &tree ) { // copy =
        SearchTree copy {tree};
        swap ( tree );

        return *this;
    }

    // auto lhs = std::move(rhs);
    // auto lhs{std::move(rhs)};
    // auto lhs(std::move(rhs));
    SearchTree ( SearchTree &&rhs ) : nodes_( std::move ( rhs.nodes_ ) ), min_key_node_( std::move ( rhs.min_key_node_ ) ),
                                      top_( std::move ( rhs.top_ ) ) {} // move ctor

    // auto lhs = ....;
    // ....
    // lhs = std::move(rhs);
    SearchTree operator= ( SearchTree &&rhs ) // move =
    {
        swap ( rhs );
        return *this;
    }

    ~SearchTree()
    {
        std::destroy ( nodes_.begin(), nodes_.end() );
    }

    size_t size() const
    {
        return nodes_.size();
    }

    iterator begin () const
    {
        return iterator ( min_key_node_ );
    }

    iterator end () const
    {
        return iterator(nullptr);
    }

    bool empty () const  { return !top_; }

    void insert ( const KeyT& value )
    {
        top_ = insert ( top_, value, nullptr );
    }

    void swap ( SearchTree& tree ) noexcept
    {
        std::swap ( top_, tree.top_ );
        std::swap ( nodes_, tree.nodes_ );
        std::swap ( min_key_node_, tree.min_key_node_ );
    }

    iterator lower_bound ( const KeyT& value ) const
    {
        Node_s* node = top_;
        Node_s* result = nullptr;

        while ( node ) {
            if ( node->key_ >= value ) {
                result = std::exchange(node, node->left_ );
            } else {
                node = node->right_;
            }
        }

        return iterator { result };
    }

    iterator upper_bound ( const KeyT& value ) const
    {
        Node_s* node = top_;
        Node_s* result = nullptr;
        while ( node ) {
            if ( node->key_ > value ) {
                result = std::exchange(node, node->left_ );
            } else {
                node = node->right_;
            }
        }

        return iterator { result };
    }

private:
    Node_s *balance ( Node_s *p )
    {
        fix_height ( p );

        const int b_factor_value = b_factor ( p );
        if ( b_factor_value == 2 )
        {
            if ( b_factor ( p->right_ ) < 0 ) {
                p->right_ = rotate_right ( p->right_ );
            }
            return rotate_left ( p );
        }
        if ( b_factor_value == -2 )
        {
            if ( b_factor ( p->left_ ) > 0  ) {
                p->left_ = rotate_left ( p->left_ );
            }
            return rotate_right ( p );
        }

        return p;
    }

    auto insert ( Node_s *node, const KeyT &key, Node_s* parent)
    {
        if ( !node ) {
            auto new_node = new Node_s( key, parent );
            nodes_.push_back ( new_node );
            if (!min_key_node_ || key < min_key_node_->key_) {
                min_key_node_ = new_node;
            }

            return new_node;
        }

        if ( key < node->key_ ) {
            node->left_ = insert ( node->left_, key, node );
            fix_height ( node->left_ );
        }
        else if ( node->key_ < key ) {
            node->right_ = insert ( node->right_, key, node );
            fix_height ( node->right_ );
        }

        return balance ( node );
    }

    void fix_height ( Node_s *p )
    {
        int hl = height ( p->left_ );
        int hr = height ( p->right_ );

        p->height_ = 1 + std::max ( hl, hr );
    }

    int height ( const Node_s *p ) const
    {
        return p ? p->height_ : 0;
    }

    int b_factor ( const Node_s *p ) const
    {
        return height ( p->right_ ) - height ( p->left_ );
    }


    Node_s *rotate_left ( Node_s *q )
    {
        Node_s *p = q->right_; // copy
        q->right_ = p->left_;
        if (p->left_) p->left_->parent_ = q;
        p->left_ = q;
        p->parent_ = q->parent_;
        q->parent_ = p;

        fix_height ( q );
        fix_height ( p );

        return p;
    }

    Node_s *rotate_right ( Node_s *node )
    {
        Node_s *new_node = node->left_;
        node->left_ = new_node->right_;
        if (new_node->right_) new_node->right_->parent_ = node;
        new_node->right_ = node;
        new_node->parent_ = node->parent_;
        node->parent_ = new_node;

        fix_height ( node );
        fix_height ( new_node );

        return new_node;
    }

    void graph_dump_nodes ( std::ofstream& file, Node_s *root) const
    {
        if (!root) return;

        std::stack<Node_s *> stack = {};
        stack.push(root);

        bool is_root = true;
        while (!stack.empty())
        {
            auto node = stack.top();

            file << "\"" << node << "\"" << "[shape = Mrecord, style = filled, fillcolor = lightpink "
                                    "label = \"" << node->key_ << "\"]" << std::endl;

            if (!is_root) file << "\"" << node->parent_ << "\"" << " -> " << "\"" << node << "\"" << std::endl;
            else          is_root = false;

            stack.pop();

            if ( node->right_ ) { stack.push(node->right_); }
            if ( node->left_  ) { stack.push(node->left_); }
        }
    }

public:
    void graph_dump ( std::string filename ) const
        {
            std::ofstream file ( filename );
            file << "digraph G {" << std::endl << "node [shape = record];" << std::endl;

            if ( top_ ) graph_dump_nodes (file, top_ );

            file << "}";
            file.close();

            std::string command = "dot -T png " + filename + " -o ../logs/tree.png";
            std::system ( command.c_str() );
        }

    void text_dump ( const Node_s *tree_node )
    {
        if ( tree_node == nullptr) {
            printf ( " nil " );

            return ;
        }
        printf ( " ( " );
        printf ( "%d", tree_node->key_ );

        Tree_Text_Dump ( tree_node->left_  );
        Tree_Text_Dump ( tree_node->right_ );

        printf ( " ) " );

    }
};

}
