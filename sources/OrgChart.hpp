#pragma once

#include "Node.hpp"
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <string>
#include <queue>
#include <vector>
#include <stack>

namespace ariel
{
    class OrgChart
    {
    private:
        Node *_root = nullptr; // root of chart

        Node *find(const std::string &val); // find node by value

        Node *find(const std::string &val, Node *currentNode); // find node by value

        void printChart(std::ostream &os, const Node *node, std::vector<bool> flag, int depth = 0) const; // print chart

        size_t numOfNodes() const; // number of nodes in the chart

    public:
        OrgChart() : _root(nullptr) {} // constructor

        OrgChart(const OrgChart &chart); // copy constructor

        OrgChart(OrgChart &&chart) noexcept; // move constructor

        OrgChart &operator=(const OrgChart &chart); // copy assignment operator

        OrgChart &operator=(OrgChart &&chart) noexcept; // move assignment operator
    
        ~OrgChart(); //destructor

        OrgChart &add_root(const std::string &val); //add root

        OrgChart &add_sub(const std::string &parent,const std::string & sub); //add sub

        friend std::ostream &operator<<(std::ostream &os, const OrgChart &chart); //cout operator

        class iterator
        {
        public:
            enum Order //enum for order of iterator
            {
                LEVELORDER,
                REVERSEORDER,
                PREORDER
            };

        private:
            Node *ptr_current; // pointer to current node
            Node *_root; // root of chart
            Node *prev = nullptr; // prev node
            Node *last; // last node for the iterator
            Order _type; // 0 = LEVELORDER, 1 = REVERSEORDER, 2 = PREORDER
            std::queue<ariel::Node *> Q; // queue for level order
            std::stack<ariel::Node *> S; // stack for reverse order

            void parent() // move up the current pointer to his parent
            {
                this->prev = this->ptr_current;
                this->ptr_current = this->ptr_current->_parent;
            }

            void son(size_t idx) // move up the current pointer to his son at index
            {
                this->prev = this->ptr_current;
                this->ptr_current = this->ptr_current->_children.at(idx);
            }

            void right_sibling() // move up the current pointer to his next brother
            {
                auto it = std::find(this->ptr_current->_parent->_children.begin(), this->ptr_current->_parent->_children.end(), this->ptr_current);
                int index = it - this->ptr_current->_parent->_children.begin();
                index++;
                this->prev = this->ptr_current;
                this->ptr_current = this->ptr_current->_parent->_children.at((size_t)index);
            }

        public:
            iterator(Node *root, Node *curr, Order type) : _type(type), _root(root), ptr_current(curr) // LEVEL ORDER constructor
            {
                for (ariel::Node *node : this->ptr_current->_children) // add the children of the root node
                {
                    Q.push(node);
                }
                this->last = nullptr;
            }

            iterator(Node *root, Order type) : _type(type), _root(root) // PREORDER || REVERSEORDER constructor
            {
                this->ptr_current = root;
                this->last = this->_root;          
                if (this->_root != nullptr)
                {
                    if (this->_type == Order::PREORDER)
                    {
                        while (!this->last->_children.empty())
                        {
                            this->last = this->last->_children.at(this->last->_children.size() - 1); // set the last node to be the right most node
                        }
                    }
                    if (this->_type == Order::REVERSEORDER)
                    {
                        this->last = _root; // set the last node to be the root node
                        std::queue<ariel::Node *> q; // queue for reverse order
                        ariel::Node *n = this->_root; 
                        q.push(n);
                        while (!q.empty())
                        {
                            S.push(q.front()); // add the front node from the queue to the stack
                            n = q.front();
                            q.pop(); // remove the node from the queue
                            for (auto it = n->_children.rbegin(); it != n->_children.rend(); ++it) // add the children to the queue in reverse order from right most child to left most
                            {
                                q.push(*it);
                            }
                        }
                        this->ptr_current = S.top(); // move the current pointer to the top of the stack
                        S.pop();
                    }
                }
            }

            Node &Node()
            {
                return *(this->ptr_current);
            }

            bool has_children() // check if a node has children
            {
                return !this->ptr_current->_children.empty();
            }

            bool has_brothers() // check if a node has brothers
            {
                std::vector<ariel::Node *> temp = this->ptr_current->_parent->_children;
                std::vector<ariel::Node *>::iterator it = std::find(temp.begin(), temp.end(), this->ptr_current);
                it++;
                return (it != temp.end());
            }

            std::string &operator*() const // * operator
            {
                return ptr_current->_value;
            }

            std::string *operator->() const // -> operator
            {
                return &(ptr_current->_value);
            }

            bool operator==(const iterator &iter) const // == operator
            {
                return (ptr_current == iter.ptr_current);
            }

            bool operator!=(const iterator &iter) const // =! operator
            {
                return (ptr_current != iter.ptr_current);
            }

            iterator &operator++() // operator ++ prefix
            {
                if (this->ptr_current == this->last) // reached the end of the iterator
                {
                    this->ptr_current = nullptr;
                    return *this;
                }

                switch (this->_type) // switch case for each type of iterator
                {
                case Order::LEVELORDER:
                {
                    if (!Q.empty())
                    {
                        this->ptr_current = Q.front(); // move the current pointer to the first node in the queue
                        Q.pop(); // remove the node from the queue
                        for (ariel::Node *node : this->ptr_current->_children) // enter all the children of the current node to the queue
                        {
                            Q.push(node);
                        }
                    }
                    else
                    {
                        this->ptr_current = nullptr; 
                    }
                    break;
                }
                case Order::REVERSEORDER:
                {
                    this->ptr_current = S.top(); // move the current pointer to the first node in the stack
                    S.pop(); // remove the node from the stack
                    break;
                }
                case Order::PREORDER:
                {
                    if (has_children()) // move the current pointer to his first son
                    {
                        son(0);
                        break;
                    }
                    while (this->ptr_current->_parent != nullptr)
                    {
                        if (has_brothers()) //moves the current pointer to his brother if he has any
                        {
                            right_sibling();
                        }
                        else
                        {
                            parent(); // go up to his parent
                            if (has_brothers()) // if he has brothers go to his right brother
                            {
                                right_sibling();
                            }
                            else{
                                continue; // keep going up if no brothers
                            }
                        }
                        break;
                    }
                    break;
                }
                }
                return *this;
            }

            iterator operator++(int) // operator++ postfix
            {
                iterator temp{this->_root, this->ptr_current, this->_type};
                this->operator++();
                return temp;
            }
        };

        iterator begin_level_order() // begin iterator for level order
        {
            if (this->_root == nullptr)
            {
                throw std::invalid_argument("Empty OrgChart");           
            }
            return iterator(_root, _root, iterator::Order::LEVELORDER);
        }

        iterator end_level_order() // end iterator for level order
        {
            if (this->_root == nullptr)
            {
                throw std::invalid_argument("Empty OrgChart");
            }
            return iterator(nullptr, iterator::Order::LEVELORDER);
        }

        iterator begin_reverse_order() // begin iterator for reverse level order
        {
            if (this->_root == nullptr)
            {
                throw std::invalid_argument("Empty OrgChart");
            }
            return iterator(this->_root, iterator::Order::REVERSEORDER);
        }

        iterator reverse_order() // end iterator for reverse level order
        {
            if (this->_root == nullptr)
            {
                throw std::invalid_argument("Empty OrgChart");
            }
            return iterator(nullptr, iterator::Order::REVERSEORDER);
        }

        iterator begin_preorder() // begin iterator for preorder
        {
            
            if (this->_root == nullptr)
            {
                throw std::invalid_argument("Empty OrgChart");
            }
            return iterator(this->_root, iterator::Order::PREORDER);
        }

        iterator end_preorder() // end iterator for preorder
        {
            if (this->_root == nullptr)
            {
                throw std::invalid_argument("Empty OrgChart");
            }
            return iterator(nullptr, iterator::Order::PREORDER);
        }

        iterator begin() // same as level order
        {
            return this->begin_level_order();
        }

        iterator end() // same as level order
        {
            return this->end_level_order();
        }
    };
}