#include "OrgChart.hpp"

using namespace std;

namespace ariel
{
    // OrgChart functions
    Node *OrgChart::find(const std::string &val) // find a node by name
    {
        if (this->_root == nullptr)
        {
            throw std::invalid_argument("root is null");
        }
        return find(val, this->_root);
    }

    // https://stackoverflow.com/questions/20735708/how-to-get-an-element-in-n-ary-trees
    Node *OrgChart::find(const std::string &val, Node *currentNode) // find a node by name
    {
        if (currentNode == nullptr)
        {
            return nullptr;
        }
        if (currentNode->_value == val) // if we found the node
        {
            return currentNode;
        }
        for (Node *node : currentNode->_children)
        {
            Node *found = find(val, node); // recursive call on each child
            if (found != nullptr)
            {
                return found;
            }
        }
        return nullptr;
    }

    // https://www.geeksforgeeks.org/print-n-ary-tree-graphically/
    void OrgChart::printChart(std::ostream &os, const Node *node, vector<bool> flag, int depth) const
    {
        if (node == nullptr)
        {
            return;
        }
        for (size_t i = 1; i < depth; i++)
        {
            os << "    ";
        }
        if (depth == 0)
        {
            os << node->_value << endl;
        }
        else if ((*node).hasBrothers())
        {
            os << "+--- " << node->_value << endl;
            flag[(size_t)depth] = false;
        }
        else
        {
            os << "+--- " << node->_value << endl;
        }
        for (auto it = node->_children.begin(); it != node->_children.end(); it++)
        {
            printChart(os, *it, flag, depth + 1);
        }
        flag[(size_t)depth] = true;
    }

    OrgChart::OrgChart(const OrgChart &chart)
    {
        this->_root = chart._root;
    }

    OrgChart::OrgChart(OrgChart &&chart) noexcept
    {
        this->_root = chart._root;
        chart._root = nullptr;
    }

    OrgChart &OrgChart::operator=(const OrgChart &chart)
    {
        if (&chart != this)
        {
            this->_root = chart._root;
        }
        return *this;
    }

    OrgChart &OrgChart::operator=(OrgChart &&chart) noexcept
    {
        this->_root = chart._root;
        chart._root = nullptr;
        return *this;
    }

    OrgChart::~OrgChart()
    {
        delete this->_root;
    }

    OrgChart &OrgChart::add_root(const string & val)
    {
        if (this->_root == nullptr)
        {
            this->_root = new Node(val); // create new root
        }
        this->_root->_value = val; // if the root already exists, change its value
        return *this;
    }

    OrgChart &OrgChart::add_sub(const string & parent, const string & sub)
    {
        Node *parentNode = find(parent); // make sure the parent is in the chart
        if (parentNode == nullptr)
        {
            throw std::invalid_argument("First value is not in the chart");
        }
        parentNode->add_child(sub);
        return *this;
    }

    // https://www.geeksforgeeks.org/number-children-given-node-n-ary-tree/
    size_t numberOfChildren(Node *root, const string & x)
    {
        size_t numChildren = 0;
        if (root == NULL)
        {
            return 0;
        }
        queue<Node *> q;
        q.push(root);
        while (!q.empty())
        {
            int n = q.size();
            while (n > 0)
            {
                Node *p = q.front();
                q.pop();
                if (p->_value == x)
                {
                    numChildren = numChildren + p->_children.size();
                    return numChildren;
                }
                for (size_t i = 0; i < p->_children.size(); i++){
                    q.push(p->_children[i]);
                }
                n--;
            }
        }
        return numChildren;
    }

    std::ostream &operator<<(std::ostream &os, const OrgChart &chart)
    {
        size_t numOfNodes = numberOfChildren(chart._root, chart._root->_value);
        std::vector<bool> flag(numOfNodes, true);
        chart.printChart(os, chart._root, flag);
        os << std::endl;
        return os;
    }

}