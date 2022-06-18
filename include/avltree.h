#pragma once

#include <initializer_list>
#include <numeric>
#include<limits>
#include <iostream>
#include <functional>

template<typename T, typename D>
class Node;

template<typename T, typename D>
class AVLTree
{
public :

  AVLTree();
  AVLTree(const AVLTree& source);
  AVLTree(std::initializer_list<std::pair<T, D>> source);
  ~AVLTree();

  AVLTree& operator = (const AVLTree & source);

  void insert(const T& key, const D& data);

  void remove(const T& key);

  D& find(const T& key);

  int size();

  int height();

  void clear();

  friend std::ostream& operator <<(std::ostream& os, const AVLTree& tree);

  void inOrderTraverse(const std::function<void(const T&, const D&)>& func);

private:

  int getNodeHeight(Node<T,D>* n);

  int getNodeBalanceFactor(Node<T,D>* n);

  void updateNodeHeight(Node<T,D>* node);

  Node<T,D>* rotateNodeLeft(Node<T,D>* node);

  Node<T,D>* rotateNodeRight(Node<T,D>* node);

  Node<T,D>* checkNodeBalance(Node<T,D>* node);

  Node<T,D>* deleteNode(Node<T,D>* node);

  // Recursive methods
  Node<T,D>* insertNode (Node<T,D>* node, const T& key, const D& data);
  void dumpNode (Node<T,D>* node, std::ostream& os) const;
  Node<T,D>* removeNode(Node<T,D>* node, const T& key);
  Node<T,D>* findIOP(Node<T,D>* node, Node<T,D>*& iop, bool balanceChecking);
  Node<T,D>* findNode(Node<T,D>* node, const T& key);
  void deleteAll(Node<T,D>* node);
  Node<T,D>* copyAll(Node<T,D>* source);
  void inOrder(Node<T,D>* n, const std::function<void(const T&, const D&)>& func);

private:

  Node<T,D>* m_Root;
};

#include "avltree.inl"