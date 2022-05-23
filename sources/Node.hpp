#pragma once

#include <string>
#include <vector>

// based on https://www.geeksforgeeks.org/insertion-in-n-ary-tree-in-given-order-and-level-order-traversal/

namespace ariel
{
    class Node
    {
    public:
        std::string _value; // value of the node
        Node *_parent; // pointer to the parent node
        std::vector<Node *> _children; // vector of pointers to the children nodes

        bool hasBrothers() const // check if a node has brothers
        {
            std::vector<Node *> temp = this->_parent->_children;
            std::vector<Node *>::iterator it = std::find(temp.begin(), temp.end(), this);
            it++;
            return (it != temp.end());
        }

        void add_child(std::string val) // add child to current node
        {
            Node *newNode = new Node(std::move(val));
            newNode->_parent = this;
            this->_children.push_back(newNode);
        }

        Node() : _parent(nullptr) {} // constructors
        Node(std::string val) : _value(std::move(val)), _parent(nullptr) {}

        ~Node() // node destructor
        {
            for (Node *node : this->_children)
            {
                delete node;
            }
        }   

        Node(const Node &node){ // copy constructor
            this->_value = node._value;
            this->_parent = node._parent;
            this->_children = node._children;
        }

        Node &operator=(const Node &node){ // move constructor
            if (&node != this)
            {
                this->_value = node._value;
                this->_parent = node._parent;
                this->_children = node._children;
            }
            return *this;
        }

        Node(Node &&node) noexcept : _value(std::move(node._value)), // copy assignment operator
                                    _children(std::move(node._children)),
                                    _parent(node._parent) {} 

        Node &operator=(Node &&node) noexcept { // move assignment operator
            this->_value = node._value;
            this->_parent = node._parent;
            this->_children = node._children;
            return *this;
        }
    };
}