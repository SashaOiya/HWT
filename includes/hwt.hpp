#pragma once 

#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <iterator>
#include <memory>
#include "hwt_log.hpp"

namespace Trees {

const int DUMP_COUNTER = 100;

enum errors {
    no_errors = 1,
    input_error = 2
};

template <typename KeyT, typename Comp>
class SearchTree {
    struct Node_s {
        KeyT key_;
        Node_s *parent_ = nullptr, *left_ = nullptr, *right_ = nullptr;
        int height_ = 0;
        Node_s(const KeyT& val, Node_s* parent = nullptr) : key_(val), parent_(parent) {}
        ~Node_s() { delete left_; delete right_; } // aaaaaaaaaaaa
    };
    const char *dump_file_name = "tree.dot";
    // deque вызовов, deque элементов

class AVLTreeIterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = KeyT;
    using difference_type = std::ptrdiff_t;
    using pointer = KeyT*;
    using reference = KeyT&;

    // Конструктор итератора
    explicit AVLTreeIterator(Node_s* node = nullptr) : current(node) {}

    // Оператор разыменования
    KeyT& operator*() const {
        return current->key_;
    }

    // Оператор префиксного инкремента
    AVLTreeIterator& operator++() {
        advanceForward();
        return *this;
    }

    // Оператор постфиксного инкремента
    AVLTreeIterator operator++(int) {
        auto temp = *this;
        advanceForward();
        return temp;
    }

    // Оператор префиксного декремента
    AVLTreeIterator& operator--() {
        advanceBackward();
        return *this;
    }

    // Оператор постфиксного декремента
    AVLTreeIterator operator--(int) {
        auto temp = *this;
        advanceBackward();
        return temp;
    }

    // Операторы сравнения на равенство/неравенство
    bool operator==(const AVLTreeIterator& other) const {
        return current == other.current;
    }

    bool operator!=(const AVLTreeIterator& other) const {
        return current != other.current;
    }

private:
    Node_s* current;

    // Метод для перехода к следующему узлу в порядке in-order
    void advanceForward() {
        if (current->right_) {
            current = current->right_;
            while (current->left_) {
                current = current->left_;
            }
        } else {
            Node_s* parent = current->parent_;
            while (parent && current == parent->right_) {
                current = parent;
                parent = parent->parent_;
            }
            current = parent;
        }
    }

    // Метод для перехода к предыдущему узлу в порядке in-order
    void advanceBackward() {
        if (current->left_) {
            current = current->left_;
            while (current->right_) {
                current = current->right_;
            }
        } else {
            Node_s* parent = current->parent_;
            while (parent && current == parent->left_) {
                current = parent;
                parent = parent->parent_;
            }
            current = parent;
        }
    }

    std::unique_ptr<Node_s> top_;
};

public: // селекторы
    //iterator lower_bound(KeyT key) const;
    //iterator upper_bound(KeyT key) const;
    //int distance(iterator fst, iterator snd) const;
    Node_s *top_ = nullptr;
    using iterator = AVLTreeIterator;

    SearchTree() : top_(nullptr) {}

    iterator begin() {
        return iterator(findMin(top_));
    }

    iterator end() {
        return iterator(nullptr);
    }

    void insert(const KeyT& value) {
        top_ = insert( top_, value, nullptr);
    }

    errors read_data () // to ctor
    {        
        char type = 0;
        do {
            std::cin >> type;
            //errors
            if ( type == 'k' ) {
                KeyT key = 0;
                std::cin >> key;
                insert ( key );
            }
            else if ( type == 'q' ) {
                KeyT key1 = 0;
                KeyT key2 = 0;

                std::cin >> key1 >> key2;
                //range_query();
            }
            else if ( type ) { return input_error; } // throw
        } while ( type );

        return no_errors;
    }

    //void insert(KeyT key);
public: // модификаторы
    /*int range_query ( T fst, T snd ) {
        using itt = typename C::iterator;
        itt start = s.lower_bound(fst); // first not less then fst
        itt fin = s.upper_bound(snd); // first greater then snd
        return mydistance(s, start, fin); // std::distance для set
    }*/
private:

    auto findMin(Node_s* node) const {
        while (node && node->left_) {
            node = node->left_;
        }
        return node;
    }

    Node_s *balance ( Node_s *p ) // балансировка узла p
    {
        fixheight ( p );

        if ( bfactor ( p ) == 2 )
        {
            if ( bfactor ( p->right_ ) < 0 ) {
                p->right_ = rotateright ( p->right_ );
            }
            return rotateleft ( p );
        }
        if ( bfactor ( p ) == -2 )
        {
            if ( bfactor ( p->left_ ) > 0  ) {
                p->left_ = rotateleft ( p->left_ );
            }
            return rotateright ( p );
        }

        return p; // балансировка не нужна
    }

    Node_s *insert ( Node_s *node, KeyT key, Node_s* parent) // вставка ключа k в дерево с корнем p
    {
        if ( !node ) { return new Node_s( key, parent ); };

        if ( key < node->key_ ) {
            node->left_  = insert ( node->left_, key, node );
            fixheight ( node->left_ );
        }
        else if ( key > node->key_ ) {
            node->right_ = insert ( node->right_, key, node );
            fixheight ( node->right_ );
        }

        return balance ( node );
    }

    void fixheight ( Node_s *p ) // name
    {
        int hl = height ( p->left_ ); // unsigned char
        int hr = height ( p->right_ );

        p->height_ = ( hl > hr ? hl : hr ) + 1;
    }

    int height ( Node_s *p )
    {
        return p ? p->height_ : 0;
    }

    int bfactor ( Node_s *p )
    {
        return height ( p->right_ ) - height ( p->left_ );
    }

    Node_s *rotateleft ( Node_s *q ) // левый поворот вокруг q
    {
        Node_s *p = q->right_; // copy
        q->right_ = p->left_;
        if (p->left_) p->left_->parent_ = q;
        p->left_ = q;
        p->parent_ = q->parent_;
        q->parent_ = p;

        fixheight ( q );
        fixheight ( p );

        return p;
    }

    Node_s *rotateright ( Node_s *node ) // правый поворот вокруг p
    {
        Node_s *new_node = node->left_;
        node->left_ = new_node->right_;
        if (new_node->right_) new_node->right_->parent_ = node;
        new_node->right_ = node;
        new_node->parent_ = node->parent_;
        node->parent_ = new_node;

        fixheight ( node );
        fixheight ( new_node );

        return new_node;
    }
public:
    void Tree_Dump_Body ( const Node_s *node, FILE *tree_dump_f ) 
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

        Tree_Dump_Body ( node->left_,  tree_dump_f );
        Tree_Dump_Body ( node->right_, tree_dump_f );
    }
    void Tree_Graph_Dump () 
    {
        FILE *tree_dump = fopen ( dump_file_name, "w" );
        if ( !tree_dump ) {
            perror ( "File opening failed" );
            return ;
        }
        fprintf ( tree_dump, "digraph G { \n"
                             "node [shape = record];\n"
                             " \"%p\" ", top_ );

        Tree_Dump_Body ( top_, tree_dump );

        fprintf ( tree_dump, "}\n" );
        fclose ( tree_dump );

        static int file_counter = 0;
        char command_buffer[DUMP_COUNTER] = {};
        fprintf ( log(), "<img src=\"tree%d.png\" alt=\"-\" width=\"500\" height=\"600\">\n", file_counter );
        sprintf ( command_buffer, "dot -T png tree.dot -o logs/tree%d.png", file_counter++ );
        std::system  ( command_buffer );
    }

    void Tree_Text_Dump ( const Node_s *tree_node ) 
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