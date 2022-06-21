#pragma once

#include <initializer_list>
#include <numeric>
#include<limits>
#include <iostream>
#include <functional>

template<typename T, typename D>
class Node
{
public :
  Node(const T& key, const D& data);
  Node(const Node& node);

  friend std::ostream& operator<< (std::ostream& os, const Node& node);

public :
  T m_Key;
  D m_Data;
  Node* m_Left;
  Node* m_Right;
  int m_Height;
};

template<typename T, typename D>
Node<T,D>::Node(const T& key, const D& data)
  :
  m_Key(key),
  m_Data(data),
  m_Left(nullptr),
  m_Right(nullptr),
  m_Height(0)
{
}

template<typename T, typename D>
Node<T,D>::Node (const Node& node)
{
  m_Key = node.m_Key;
  m_Data = node.m_Data;
  m_Left = node.m_Left;
  m_Right = node.m_Right;
  m_Height = node.m_Height;
}

template<typename T, typename D>
std::ostream& operator<< (std::ostream& os, const Node<T,D>& node)
{
  os << " Key :" << node.m_Key
     << " Height : " << node.m_Height
     << " Left child : " << (node.m_Left != nullptr ? node.m_Left->m_Key : -1)
     << " Right child : " << (node.m_Right != nullptr ? node.m_Right->m_Key : -1);

  return os;
}


template<typename T, typename D>
AVLTree<T,D>::AVLTree()
  : m_Root(nullptr)
{}

template<typename T, typename D>
AVLTree<T,D>::AVLTree(const AVLTree<T,D>& source)
{
  m_Root = copyAll(source.m_Root);
}

template<typename T, typename D>
AVLTree<T,D>::AVLTree(std::initializer_list<std::pair<T, D>> source)
  : m_Root(nullptr)
{
  for (auto element : source)
  {
    insert(element.first, element.second);
  }
}

template<typename T, typename D>
AVLTree<T,D>::~AVLTree()
{
  clear();
}

template<typename T, typename D>
AVLTree<T,D>& AVLTree<T,D>::operator = (const AVLTree<T,D> & source)
{
  m_Root = copyAll(source.m_Root);
  return *this;
}

template<typename T, typename D>
void AVLTree<T,D>::insert(const T& key, const D& data)
{
  m_Root = insertNode(m_Root, key, data);
}

template<typename T, typename D>
void AVLTree<T,D>::remove(const T& key)
{
  m_Root = removeNode(m_Root, key);
}

template<typename T, typename D>
D& AVLTree<T,D>::find(const T& key)
{
  Node* n = findNode(m_Root);
  return n->m_Data;
}

template<typename T, typename D>
int AVLTree<T,D>::size()
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

template<typename T, typename D>
int AVLTree<T,D>::height()
{
  return m_Root ? m_Root->m_Height : 0;
}

template<typename T, typename D>
void AVLTree<T,D>::clear()
{
  deleteAll(m_Root);
  m_Root = nullptr;
}

template< typename T, typename D>
std::ostream& operator <<(std::ostream& os, const AVLTree<T,D>& tree)
{
  tree.dumpNode(tree.m_Root, os);
  os<<std::endl;
  return os;
}

template< typename T, typename D>
void AVLTree<T,D>::inOrderTraverse(
  const std::function<void(const T&, const D&)>& func
)
{
  inOrder(m_Root, func);
}

template< typename T, typename D>
void AVLTree<T,D>::dumpNode(Node<T,D>* node, std::ostream& os) const
{
  if (node == nullptr)
  {
    return;
  }

  dumpNode(node->m_Left, os);
  os<<*node<<std::endl;
  dumpNode(node->m_Right, os);
}

template< typename T, typename D>
Node<T,D>* AVLTree<T,D>::insertNode(
  Node<T,D>* node,
  const T& key,
  const D& data
)
{
  Node<T,D>* ret = nullptr;

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
    node->m_Right = insertNode(node->m_Right, key, data);
    ret = checkNodeBalance(node);
  }
  else
  {
    node->m_Left = insertNode(node->m_Left, key, data);
    ret = checkNodeBalance(node);
  }

  return ret;
}

template< typename T, typename D>
void AVLTree<T,D>::updateNodeHeight(Node<T,D>* node)
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

template< typename T, typename D>
int AVLTree<T,D>::getNodeHeight(Node<T,D>* n)
{
  if (n == nullptr)
  {
    return -1;
  }

  return n->m_Height;
}

template< typename T, typename D>
int AVLTree<T,D>::getNodeBalanceFactor(Node<T,D>* n)
{
  if (n == nullptr)
  {
    return 0;
  }

  return getNodeHeight(n->m_Right) - getNodeHeight(n->m_Left);
}

template< typename T, typename D>
Node<T,D>* AVLTree<T,D>::rotateNodeLeft(Node<T,D>* node)
{
  if (node == nullptr)
  {
    return nullptr;
  }

  auto x = node;
  auto y = node->m_Right;
  auto z = y->m_Left;

  x->m_Right = z;
  y->m_Left = x;

  updateNodeHeight(x);
  updateNodeHeight(y);

  return y;
}

template <typename T, typename D>
Node<T,D>* AVLTree<T,D>::rotateNodeRight(Node<T,D>* node)
{
  if (node == nullptr)
  {
    return nullptr;
  }

  Node<T,D>* x = node;
  Node<T,D>* y = node->m_Left;
  Node<T,D>* z = y->m_Right;

  y->m_Right = x;
  x->m_Left = z;

  updateNodeHeight(x);
  updateNodeHeight(y);

  return y;
}

template <typename T, typename D>
Node<T,D>* AVLTree<T,D>::checkNodeBalance(Node<T,D>* node)
{
  auto ret = node;

  if (node == nullptr)
  {
    return ret;
  }

  updateNodeHeight(node);

  // Note : To balance our tree, we compute a balance factor :
  // balance factor = leftNode Height - rightNode Height

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

template <typename T, typename D>
Node<T,D>* AVLTree<T,D>::removeNode(Node<T,D>* node, const T& key)
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
    node->m_Right = removeNode(node->m_Right, key);
    return checkNodeBalance(node);
  }
  else
  {
    node->m_Left = removeNode(node->m_Left, key);
    return checkNodeBalance(node);
  }
}

template <typename T, typename D>
Node<T,D>* AVLTree<T,D>::deleteNode(Node<T,D>* node)
{
  Node<T,D>* ret = nullptr;
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
    // Note : Let's find the In Order Predecessor to replace
    // the node we are about to delete
    Node<T,D>*  iop = nullptr;
    findIOP(node, iop, false);
    ret = iop;
    ret->m_Left = node->m_Left;
    ret->m_Right = node->m_Right;
    delete node;
  }

  return ret;
}

template <typename T, typename D>
Node<T,D>* AVLTree<T,D>::findIOP(
  Node<T,D>* node,
  Node<T,D>*& iop,
  bool balanceChecking
)
{
  if (node == nullptr)
  {
    return nullptr;
  }

  if (node->m_Right != nullptr)
  {
    Node<T,D>* cnode(findIOP(node->m_Right, iop, true));
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

template <typename T, typename D>
Node<T,D>* AVLTree<T,D>::findNode(Node<T,D>* node, const T& key)
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

template <typename T, typename D>
void AVLTree<T,D>::deleteAll(Node<T,D>* node)
{
  if (node == nullptr)
  {
    return;
  }

  deleteAll(node->m_Left);
  deleteAll(node->m_Right);

  delete node;
}

template <typename T, typename D>
void AVLTree<T,D>::inOrder(
  Node<T,D>* n,
  const std::function<void(const T&, const D&)>& func
)
{
  if (n == NULL)
  {
    return;
  }

  inOrder(n->m_Left, func);

  func(n->m_Key, n->m_Data);

  inOrder(n->m_Right, func);

}

template <typename T, typename D>
Node<T,D>* AVLTree<T,D>::copyAll(Node<T,D>* source)
{
  if (source == nullptr)
  {
    return nullptr;
  }

  auto left = copyAll(source->m_Left);
  auto right = copyAll(source->m_Right);

  auto  n = new Node(source->m_Key, source->m_Data);
  n->m_Left = left;
  n->m_Right = right;

  updateNodeHeight(n);

  return n;
}

