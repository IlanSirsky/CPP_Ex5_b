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
        Node *_root = nullptr;

        Node *find(const std::string &val);

        Node *find(const std::string &val, Node *currentNode);

        void printChart(std::ostream &os, const Node *node, std::vector<bool> flag, int depth = 0) const;

        size_t numOfNodes() const;

    public:
        OrgChart() : _root(nullptr) {}

        OrgChart(const OrgChart &chart);

        OrgChart(OrgChart &&chart) noexcept;

        OrgChart &operator=(const OrgChart &chart);

        OrgChart &operator=(OrgChart &&chart) noexcept;
    
        ~OrgChart();

        OrgChart &add_root(const std::string &val);

        OrgChart &add_sub(const std::string &parent,const std::string & sub);

        friend std::ostream &operator<<(std::ostream &os, const OrgChart &chart);

        class iterator
        {
        public:
            enum Order
            {
                LEVELORDER,
                REVERSEORDER,
                PREORDER
            };

        private:
            Node *ptr_current;
            Node *_root;
            Node *prev = nullptr;
            Node *last;
            Order _type; // 0 = LEVELORDER, 1 = REVERSEORDER, 2 = PREORDER
            std::queue<ariel::Node *> Q;
            std::stack<ariel::Node *> S;

            void parent()
            {
                this->prev = this->ptr_current;
                this->ptr_current = this->ptr_current->_parent;
            }

            void son(size_t idx)
            {
                this->prev = this->ptr_current;
                this->ptr_current = this->ptr_current->_children.at(idx);
            }

            void right_sibling()
            {
                auto it = std::find(this->ptr_current->_parent->_children.begin(), this->ptr_current->_parent->_children.end(), this->ptr_current);
                int index = it - this->ptr_current->_parent->_children.begin();
                index++;
                this->prev = this->ptr_current;
                this->ptr_current = this->ptr_current->_parent->_children.at((size_t)index);
            }

        public:
            iterator(Node *root, Node *curr, Order type) : _type(type), _root(root), ptr_current(curr) //LEVEL ORDER
            {
                this->last = this->_root;
                for (ariel::Node *node : this->ptr_current->_children)
                {
                    Q.push(node);
                }
                this->last = nullptr;
            }

            iterator(Node *root, Order type) : _type(type), _root(root) //PREORDER || REVERSEORDER
            {
                this->ptr_current = root;
                this->last = this->_root;          
                if (this->_root != nullptr)
                {
                    if (this->_type == Order::PREORDER)
                    {
                        while (!this->last->_children.empty())
                        {
                            this->last = this->last->_children.at(this->last->_children.size() - 1);
                        }
                    }
                    if (this->_type == Order::REVERSEORDER)
                    {
                        this->last = _root;
                        std::queue<ariel::Node *> q;
                        ariel::Node *n = this->_root;
                        q.push(n);
                        while (!q.empty())
                        {
                            S.push(q.front());
                            n = q.front();
                            q.pop();
                            for (auto it = n->_children.rbegin(); it != n->_children.rend(); ++it)
                            {
                                q.push(*it);
                            }
                        }
                        this->ptr_current = S.top();
                        S.pop();
                    }
                }
            }

            Node &Node()
            {
                return *(this->ptr_current);
            }

            bool has_children()
            {
                return !this->ptr_current->_children.empty();
            }

            bool has_brothers()
            {
                std::vector<ariel::Node *> temp = this->ptr_current->_parent->_children;
                std::vector<ariel::Node *>::iterator it = std::find(temp.begin(), temp.end(), this->ptr_current);
                it++;
                return (it != temp.end());
            }

            std::string &operator*() const
            {
                return ptr_current->_value;
            }

            std::string *operator->() const
            {
                return &(ptr_current->_value);
            }

            bool operator==(const iterator &iter) const
            {
                return (ptr_current == iter.ptr_current);
            }

            bool operator!=(const iterator &iter) const
            {
                return (ptr_current != iter.ptr_current);
            }

            iterator &operator++()
            {
                if (this->ptr_current == this->last)
                {
                    this->ptr_current = nullptr;
                    return *this;
                }

                switch (this->_type)
                {
                case Order::LEVELORDER:
                {
                    if (!Q.empty())
                    {
                        this->ptr_current = Q.front();
                        Q.pop();
                        for (ariel::Node *node : this->ptr_current->_children)
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
                    this->ptr_current = S.top();
                    S.pop();
                    break;
                }
                case Order::PREORDER:
                {
                    if (has_children())
                    {
                        son(0);
                        break;
                    }
                    while (this->ptr_current->_parent != nullptr)
                    {
                        if (has_brothers())
                        {
                            right_sibling();
                        }
                        else
                        {
                            parent();
                            if (has_brothers())
                            {
                                right_sibling();
                            }
                            else{
                                continue;
                            }
                        }
                        break;
                    }
                    break;
                }
                }
                return *this;
            }

            iterator operator++(int)
            {
                iterator temp{this->_root, this->ptr_current, this->_type};
                this->operator++();
                return temp;
            }
        };

        iterator begin_level_order()
        {
            if (this->_root == nullptr)
            {
                throw std::invalid_argument("Empty OrgChart");           
            }
            return iterator(_root, _root, iterator::Order::LEVELORDER);
        }

        iterator end_level_order()
        {
            if (this->_root == nullptr)
            {
                throw std::invalid_argument("Empty OrgChart");
            }
            return iterator(nullptr, iterator::Order::LEVELORDER);
        }

        iterator begin_reverse_order()
        {
            if (this->_root == nullptr)
            {
                throw std::invalid_argument("Empty OrgChart");
            }
            return iterator(this->_root, iterator::Order::REVERSEORDER);
        }

        iterator reverse_order()
        {
            if (this->_root == nullptr)
            {
                throw std::invalid_argument("Empty OrgChart");
            }
            return iterator(nullptr, iterator::Order::REVERSEORDER);
        }

        iterator begin_preorder()
        {
            
            if (this->_root == nullptr)
            {
                throw std::invalid_argument("Empty OrgChart");
            }
            return iterator(this->_root, iterator::Order::PREORDER);
        }

        iterator end_preorder()
        {
            if (this->_root == nullptr)
            {
                throw std::invalid_argument("Empty OrgChart");
            }
            return iterator(nullptr, iterator::Order::PREORDER);
        }

        iterator begin()
        {
            return this->begin_level_order();
        }
        iterator end()
        {
            return iterator(nullptr, iterator::Order::LEVELORDER);
        }
    };
}