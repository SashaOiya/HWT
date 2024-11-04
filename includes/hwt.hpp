#pragma once 

#include <iostream>
#include <vector>
#include "hwt_log.hpp"
#include "node_iterator.hpp"

namespace Trees {

template <typename KeyT, typename Comp>
class SearchTree {
    const char *dump_file_name = "tree.dot";
    using iterator = hwt::iterator<KeyT>;
    using Node_s   = hwt::Node_s<KeyT>;
    Node_s *top_ = nullptr;

    Node_s *min_key_node_ = nullptr;

public: 
    std::vector <Node_s *> nodes_ {};

    SearchTree() : top_(nullptr) {}

    SearchTree(const SearchTree &tree) : top_(nullptr) 
    {
        nodes_.reserve ( tree.size() );

        for ( auto &i : tree.nodes_ ){
            insert ( i->key_ );
        }
    }

    SearchTree& operator=(const SearchTree &tree )
    {
        SearchTree copy {tree};
        swap ( tree );

        return *this;
    }

    ~SearchTree() = default;

    const size_t size() const
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

    bool empty () const 
    {
        return !top_;
    }

    void insert ( const KeyT& value ) 
    {
        top_ = insert ( top_, value, nullptr );
    }
    
    void swap ( SearchTree& tree ) noexcept
    {
        std::swap ( this->top_, tree.top_ );
        std::swap ( this->nodes_, tree.nodes_ );
    }

    iterator lower_bound ( const KeyT& value ) const
    {
        Node_s* node = top_;
        Node_s* result = nullptr;

        while ( node ) {
            if ( node->key_ >= value ) {
                result = node; 
                node = node->left_;
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
                result = node;  
                node = node->left_;
            } else {
                node = node->right_;
            }
        }

        return iterator { result };
    }

    size_t range_query ( const KeyT& fst, const KeyT& snd ) const
    {
        auto start = lower_bound ( fst ); 
        auto fin = upper_bound ( snd ); 

        return  std::distance ( start, fin );
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

    auto insert ( Node_s *node, const KeyT key, Node_s* parent)
    {
        if ( !node ) { 
            auto new_node = new Node_s( key, parent ); 
            nodes_.push_back ( new_node ); 
            min_key_node_ = ( min_key_node_  && min_key_node_->key_ < key ) ? min_key_node_ : new_node;  
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

        p->height_ = std::max ( hl, hr ) + 1;
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

    static const int kDumpCounter = 100;

    void graph_dump_body ( const Node_s *node, FILE *tree_dump_f )  // fprintf
    {
        if ( !node ) { return ; }

        fprintf ( tree_dump_f , " \"%p\" [shape = Mrecord, style = filled, fillcolor = lightpink "
                                " label = \"data: %d \"];\n", node, node->key_ );
        if ( node->left_ ) {
            fprintf ( tree_dump_f, "\"%p\" -> \"%p\" ", node, node->left_ );
        }
        if ( node->right_ ) {
            fprintf ( tree_dump_f, "\n \"%p\" -> \"%p\" \n", node, node->right_ );
        }

        graph_dump_body ( node->left_,  tree_dump_f );
        graph_dump_body ( node->right_, tree_dump_f );
    }

public:
    void graph_dump () 
    {
        FILE *tree_dump = fopen ( dump_file_name, "w" );
        if ( !tree_dump ) {
            perror ( "File opening failed" );
            return ;
        }
        fprintf ( tree_dump, "digraph G { \n"
                             "node [shape = record];\n"
                             " \"%p\" ", top_ );

        graph_dump_body ( top_, tree_dump );

        fprintf ( tree_dump, "}\n" );
        fclose ( tree_dump );

        static int file_counter = 0;
        char command_buffer[kDumpCounter] = {};
        fprintf ( log(), "<img src=\"tree%d.png\" alt=\"-\" width=\"500\" height=\"600\">\n", file_counter );
        sprintf ( command_buffer, "dot -T png tree.dot -o logs/tree%d.png", file_counter++ );
        std::system  ( command_buffer );
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