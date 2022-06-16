#pragma once

#include <initializer_list>
#include <numeric>
#include<limits>
#include <iostream>

template<typename T, typename D>
class AVLTree
{
    public :

    AVLTree()
    : m_Root(nullptr)
    {}

    AVLTree(const AVLTree& source)
    {
        m_Root = copyAll(source.m_Root);
    }

    AVLTree(std::initializer_list<std::pair<T, D>> source)
    : m_Root(nullptr)
    {
        for (auto element : source)
            insert(element.first, element.second);
    }

    ~AVLTree()
    {
        clear();
    }

    AVLTree& operator =(const AVLTree & source)
    {
        m_Root = copyAll(source.m_Root);
        return *this;
    }

    void insert(const T& key, const D& data)
    {
        m_Root = insert_(m_Root, key, data);
    }

    void remove(const T& key)
    {
        m_Root = remove_(m_Root, key);
    }

    D& find(const T& key)
    {
        Node* n = find_(m_Root);
        return n->m_Data;
    }

    int size()
    {
    //     int size = 0;
    //     postOrder
    //     (
    //         m_Root,
    //         [&] (Node* n)
    //         {
    //             ++size;
    //         }
    //     );

    //     return size;

        return 0;
    }

    void clear()
    {
        deleteAll(m_Root);
        m_Root = nullptr;
    }

    friend std::ostream& operator <<(std::ostream& os, const AVLTree & tree)
    {
        tree.dumpNode(tree.m_Root, os);
        os<<std::endl;
        return os;
    }

    private :

    class Node 
    {
    public :
        Node (const T& key, const D& data)
        :
        m_Key(key), 
        m_Data(data), 
        m_Left(nullptr), 
        m_Right(nullptr),
        m_Height(0)
        {
        }

        Node (const Node& node)
        {
            m_Key = node.m_Key;
            m_Data = node.m_Data;
            m_Left = node.m_Left;
            m_Right = node.m_Right;
            m_Height = node.m_Height;
        }

        ~Node ()
        {
        }

        friend std::ostream& operator<< (std::ostream& os, const Node& node)
        {
            os  <<" Key :" << node.m_Key 
                <<" Height : " << node.m_Height
                <<" Left child : " << (node.m_Left != nullptr ? node.m_Left->m_Key : -1)
                <<" Right child : " << (node.m_Right != nullptr ? node.m_Right->m_Key : -1);

            return os;
        }

        T                       m_Key;
        D                       m_Data;
        Node*                   m_Left;
        Node*                   m_Right;
        int                     m_Height;
    };

private:

    void dumpNode (Node* node, std::ostream& os) const
    {
        if (node == nullptr)
            return;

        dumpNode(node->m_Left, os);
        os<<*node<<std::endl;
        dumpNode(node->m_Right, os);
    }

    Node* insert_ (Node* node, const T& key, const D& data)
    {
        Node* ret = nullptr;

        if (node == nullptr)
        {
            ret = new Node(key, data);
        }
        else if (key == node->m_Key)
        {
            ret = node;
        }   
        else if (key > node->m_Key)
        {
            node->m_Right = insert_(node->m_Right, key, data);
            ret = checkBalance(node);
        }
        else
        {
            node->m_Left = insert_(node->m_Left, key, data);
            ret = checkBalance(node);   
        }

        return ret;
    }

    void updateHeight(Node* node)
    {
        if (node == nullptr)
            return;

        node->m_Height = 1 + std::max(getHeight(node->m_Left), getHeight(node->m_Right));
    }

    int getHeight(Node* n)
    {
        if (n == nullptr)
            return -1;

        return n->m_Height;
    }

    int getBalanceFactor(Node* n)
    {
        if (n == nullptr)
            return 0;

        return getHeight(n->m_Right) - getHeight(n->m_Left);
    }

    Node* rotateLeft(Node* node)
    {
        if (node == nullptr)
            return nullptr;

        Node* x = node;
        Node* y = node->m_Right;
        Node* z = y->m_Left;

        x->m_Right = z; 
        y->m_Left = x;

        updateHeight(x);
        updateHeight(y);

        return y; 
    }

    Node* rotateRight(Node* node)
    {
        if (node == nullptr)
            return nullptr;

        Node* x = node;
        Node* y = node->m_Left;
        Node* z = y->m_Right;

        y->m_Right = x;
        x->m_Left = z;

        updateHeight(x);
        updateHeight(y);
        
        return y;
    }

    Node* checkBalance(Node* node)
    {
        Node* ret = node;

        if (node == nullptr)
            return ret;

        std::cout<<"checkBalance "<<node->m_Key<<std::endl;

        updateHeight(node);

        int balanceFactor = getBalanceFactor(node);  

        if (balanceFactor == 2)
        {
            printf("inbalance right\n");
            int rChildBalanceFactor = getBalanceFactor(node->m_Right);
            printf("second  factor == %i\n", rChildBalanceFactor);
            if (rChildBalanceFactor == 1 || rChildBalanceFactor == 0)
            {
                printf("1\n");
                ret = rotateLeft(node);     
            }
            else if (rChildBalanceFactor == -1)
            {
                printf("2\n");
                rotateRight(node->m_Right);       
                ret = rotateLeft(node);
            }
        }
        else if (balanceFactor == -2)
        {
            printf("inbalance left\n");
            int lChildBalanceFactor = getBalanceFactor(node->m_Left);
            printf("second  factor == %i\n", lChildBalanceFactor);
            if (lChildBalanceFactor == 1 || lChildBalanceFactor == 0)
            {
                printf("3\n");
                rotateLeft(node->m_Left);
                ret = rotateRight(node);
            }
            else if (lChildBalanceFactor == -1)
            {
                printf("4\n");
                ret = rotateRight(node);
            }
        }

        return ret;
    }

    Node* remove_(Node* node, const T& key)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        else if (node->m_Key == key)
        {
            std::cout<<"ccoucou"<<std::endl;
            return deleteNode(node);
        }
        else if (key > node->m_Key)
        {
            node->m_Right = remove_(node->m_Right, key);
            return checkBalance(node);
        }
        else 
        {
            node->m_Left = remove_(node->m_Left, key);
            return checkBalance(node);
        }
    }

    Node* deleteNode(Node* node)
    {
        Node* ret = nullptr;
        if (node->m_Right == nullptr)
        {
            if (node->m_Left == nullptr)
            {
                // none
                std::cout<<"nada"<<std::endl;
                delete node;
            }
            else
            {
                // only left
                ret = node->m_Left;
                delete node;
            }
        }
        else if (node->m_Left == nullptr)
        {
            // only right
            ret = node->m_Right;
            delete node;
        }
        else
        {
            // left & right
            Node* iop = nullptr;  
            findIOP(node, iop, false);       
            ret = iop;
            ret->m_Left = node->m_Left;
            ret->m_Right = node->m_Right;
            delete node;      
        }

        return ret;
    }

    Node* findIOP(Node* node, Node*& iop, bool balanceChecking)
    {
        if (node == nullptr)
            return nullptr;

        if (node->m_Right != nullptr)
        {
            Node* node = findIOP(node->m_Right, iop, true);
            if (balanceChecking == true)
                checkBalance(node);
            return node;
        }
        {
            iop = node;
            return nullptr;
        }
    }

    Node* find_(Node* node, const T& key)
    {
        if (node == NULL)
            return NULL;

        if (key == node->m_Key)
            return this;
        else if (key > node->m_Key)
            return find(node->m_Right, key);
        else
            return find(node->m_Left, key);
    }

    void deleteAll(Node* node)
    {
        if (node == nullptr)
            return;

        deleteAll(node->m_Left);
        deleteAll(node->m_Right);

        delete node;
    }


    // void postOrder(Node* n, void (*func)(Node*))
    // {
    //     if (n == NULL)
    //         return;

    //     postOrder(n->m_Left, func);
    //     postOrder(n->m_Right, func);

    //     func(n);
    // }

    Node* copyAll(Node* source)
    {
        if (source == nullptr)
            return nullptr;

        Node* left = copyAll(source->m_Left);
        Node* right = copyAll(source->m_Right);

        Node* n = new Node(source->m_Key, source->m_Data);
        n->m_Left = left;
        n->m_Right = right;

        updateHeight(n);

        return n;
    }

private:

    Node* m_Root;
};