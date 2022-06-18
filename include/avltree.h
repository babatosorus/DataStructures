#pragma once

#include <initializer_list>
#include <numeric>
#include<limits>
#include <iostream>
#include <functional>

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
    {
      insert(element.first, element.second);
    }
  }

  ~AVLTree()
  {
    clear();
  }

  AVLTree& operator = (const AVLTree & source)
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
    int size = 0;
    inOrder(
      m_Root,
      [&] (const T&, const D&)
    {
      ++size;
    }
    );

    return size;
  }

  int height()
  {
    int height = 0;

    inOrder(
      m_Root,
      [&] (const T&, const D&)
    {
      std::max(height, )
    }
    );

    return size;
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

  void inOrderTraverse(const std::function<void(const T&, const D&)>& func)
  {
    inOrder(m_Root, func);
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
    {
      return;
    }

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
      ret = checkNodeBalance(node);
    }
    else
    {
      node->m_Left = insert_(node->m_Left, key, data);
      ret = checkNodeBalance(node);
    }

    return ret;
  }

  void updateNodeHeight(Node* node)
  {
    if (node == nullptr)
    {
      return;
    }

    node->m_Height = 1 + std::max(
                       getNodeHeight(node->m_Left),
                       getNodeHeight(node->m_Right)
                     );
  }

  int getNodeHeight(Node* n)
  {
    if (n == nullptr)
    {
      return -1;
    }

    return n->m_Height;
  }

  int getNodeBalanceFactor(Node* n)
  {
    if (n == nullptr)
    {
      return 0;
    }

    return getNodeHeight(n->m_Right) - getNodeHeight(n->m_Left);
  }

  Node* rotateNodeLeft(Node* node)
  {
    if (node == nullptr)
    {
      return nullptr;
    }

    Node* x = node;
    Node* y = node->m_Right;
    Node* z = y->m_Left;

    x->m_Right = z;
    y->m_Left = x;

    updateNodeHeight(x);
    updateNodeHeight(y);

    return y;
  }

  Node* rotateNodeRight(Node* node)
  {
    if (node == nullptr)
    {
      return nullptr;
    }

    Node* x = node;
    Node* y = node->m_Left;
    Node* z = y->m_Right;

    y->m_Right = x;
    x->m_Left = z;

    updateNodeHeight(x);
    updateNodeHeight(y);

    return y;
  }

  Node* checkNodeBalance(Node* node)
  {
    Node* ret = node;

    if (node == nullptr)
    {
      return ret;
    }

    updateNodeHeight(node);

    int balanceFactor = getNodeBalanceFactor(node);

    if (balanceFactor == 2)
    {
      int rChildBalanceFactor = getNodeBalanceFactor(node->m_Right);
      if (rChildBalanceFactor == 1 || rChildBalanceFactor == 0)
      {
        ret = rotateNodeLeft(node);
      }
      else if (rChildBalanceFactor == -1)
      {
        rotateNodeRight(node->m_Right);
        ret = rotateNodeLeft(node);
      }
    }
    else if (balanceFactor == -2)
    {
      int lChildBalanceFactor = getNodeBalanceFactor(node->m_Left);
      if (lChildBalanceFactor == 1 || lChildBalanceFactor == 0)
      {
        rotateNodeLeft(node->m_Left);
        ret = rotateNodeRight(node);
      }
      else if (lChildBalanceFactor == -1)
      {
        ret = rotateNodeRight(node);
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
      return deleteNode(node);
    }
    else if (key > node->m_Key)
    {
      node->m_Right = remove_(node->m_Right, key);
      return checkNodeBalance(node);
    }
    else
    {
      node->m_Left = remove_(node->m_Left, key);
      return checkNodeBalance(node);
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
    {
      return nullptr;
    }

    if (node->m_Right != nullptr)
    {
      Node* cnode(findIOP(node->m_Right, iop, true));
      if (balanceChecking == true)
      {
        checkNodeBalance(cnode);
      }
      return cnode;
    }
    {
      iop = node;
      return nullptr;
    }
  }

  Node* find_(Node* node, const T& key)
  {
    if (node == NULL)
    {
      return NULL;
    }

    if (key == node->m_Key)
    {
      return this;
    }
    else if (key > node->m_Key)
    {
      return find(node->m_Right, key);
    }
    else
    {
      return find(node->m_Left, key);
    }
  }

  void deleteAll(Node* node)
  {
    if (node == nullptr)
    {
      return;
    }

    deleteAll(node->m_Left);
    deleteAll(node->m_Right);

    delete node;
  }

  void inOrder(Node* n, const std::function<void(const T&, const D&)>& func)
  {
    if (n == NULL)
    {
      return;
    }

    inOrder(n->m_Left, func);

    func(n->m_Key, n->m_Data);

    inOrder(n->m_Right, func);

  }

  Node* copyAll(Node* source)
  {
    if (source == nullptr)
    {
      return nullptr;
    }

    Node* left = copyAll(source->m_Left);
    Node* right = copyAll(source->m_Right);

    Node* n = new Node(source->m_Key, source->m_Data);
    n->m_Left = left;
    n->m_Right = right;

    updateNodeHeight(n);

    return n;
  }

private:

  Node* m_Root;
};