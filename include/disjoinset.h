#pragma once

#include <iostream>
#include <unordered_map>

template <typename T>
class DisjoinSet
{
public :

  DisjoinSet()
    : m_Size(0), m_Capacity(0), m_Array(nullptr)
  {
  }

  ~DisjoinSet()
  {
    delete m_Array;
    m_Array = nullptr;
  }

  void resize()
  {
    if (m_Size++ < m_Capacity)
    {
      return;
    }

    if (m_Array == nullptr)
    {
      m_Capacity = 1;
      m_Array = new Data[m_Capacity];
    }
    else
    {
      m_Capacity *= 2;
      Data* newArray = new Data[m_Capacity];
      for (int i = 0; i < m_Size - 1; ++i)
      {
        newArray[i] = m_Array[i];
      }

      delete m_Array;
      m_Array = newArray;
    }

  }

  void makeSet(const T & val)
  {
    if (m_LookUp.count(val))
    {
      return;
    }

    resize();
    m_Array[m_Size -1] = Data();
    m_LookUp[val] = m_Size -1;
  }

  int find(const T& val)
  {
    auto it = m_LookUp.find(val);
    if (it == m_LookUp.end())
    {
      return -1;
    }

    return findSet(it->second);
  }

  void setUnion(const T& i, const T& j)
  {
    int id1 = find(i);
    int id2 = find(j);

    if (id1 == id2)
    {
      return;
    }

    if (m_Array[id1].Size < m_Array[id2].Size)
    {
      std::swap(id1, id2);
    }

    int size = m_Array[id2].Size;
    m_Array[id2].Parent = id1;
    m_Array[id1].Size += size;
  }

  friend std::ostream& operator <<(std::ostream& os, const DisjoinSet& k)
  {
    for (auto it = k.m_LookUp.begin(); it != k.m_LookUp.end(); ++it)
    {
      std::cout<<it->first<<": "<<it->second<<std::endl;
    }

    for (int i = 0; i < k.m_Size; ++i)
    {
      std::cout<<"[p="<<k.m_Array[i].Parent<<", s="<<k.m_Array[i].Size<<"]"<<" ";
    }

    std::cout<<std::endl;

    return os;
  }

private :

  int findSet(const int&  i)
  {
    if (i < 0 || i >= m_Size)
    {
      return -1;
    }
    else if (m_Array[i].Parent < 0)
    {
      return i;
    }
    else
    {
      int parent = findSet(m_Array[i].Parent);
      m_Array[i].Parent = parent;
      return parent;
    }
  }

  struct Data
  {
    Data()
      : Parent(-1), Size(1)
    {}

    int Parent;
    int Size;
  };

private :

  int 							m_Size;
  int 							m_Capacity;
  Data* 						m_Array;

  std::unordered_map<T, int>	m_LookUp;

};