#pragma once

#include <utility>
#include <cstring>
#include <iostream>
#include <fstream>


enum HEAP_TYPE
{
  MIN_HEAP,
  MAX_HEAP
};

template <typename T, HEAP_TYPE heap_type>
class Heap
{
public :

  Heap()
    :
    m_Array(nullptr),
    m_Capacity(0),
    m_Size(0)
  {}

  ~Heap()
  {
    clear();
  }

  void clear()
  {
    delete [] m_Array;
    m_Array = nullptr;
    m_Capacity = 0;
    m_Size = 0;
  }

  void insert(const T& val)
  {
    if (m_Size >= m_Capacity)
    {
      resize();
    }

    ++m_Size;

    m_Array[m_Size - 1] = val;
    heapifyUp(m_Size - 1);
  }

  int capacity() const
  {
    return m_Capacity;
  }

  int size() const
  {
    return m_Size;
  }

  T remove()
  {

    if (m_Size <= 0)
    {
      throw std::runtime_error("empty heap");
    }

    T ret = m_Array[0];
    m_Array[0] = m_Array[m_Size - 1];
    --m_Size;

    heapifyDown(0);

    return ret;
  }

  friend std::ostream& operator << (std::ostream& os, const Heap& k)
  {
    for (int i = 0; i < k.m_Size; i++)
    {
      std::cout<<k.m_Array[i]<< " ";
    }

    std::cout<<std::endl;

    return os;
  }

  explicit operator const T*() const
  {
    return m_Array;
  }

  void write(const char* filename)
  {
    std::ofstream outputFile;

    outputFile.open(filename,  std::ios::binary | std::ios::out);

    if (!outputFile.is_open())
    {
      return;
    }

    outputFile.write(reinterpret_cast<char*>(m_Array), sizeof(T)*m_Size);

    outputFile.close();
  }

  void load(const char* filename)
  {
    std::ifstream inputFile;
    inputFile.open(filename, std::ios::binary | std::ios::ate);

    if (!inputFile.is_open())
    {
      return;
    }

    clear();

    std::ifstream::pos_type pos = inputFile.tellg();
    int lenght = pos;
    m_Size = lenght/sizeof(T);
    m_Capacity = m_Size;

    if(m_Size <= 0)
    {
      return;
    }

    m_Array = new T[m_Size];

    inputFile.seekg(0, std::ios::beg);
    inputFile.read(reinterpret_cast<char*>(m_Array), lenght);

    inputFile.close();
  }

private :

  void resize()
  {
    if (m_Array == nullptr)
    {
      m_Capacity = 1;
      m_Array = new T [m_Capacity];
    }
    else
    {
      m_Capacity *= 2;
      T* newArray = new T [m_Capacity];
      for (int i = 0; i < m_Size; i++)
      {
        newArray[i] = m_Array[i];
      }
      delete [] m_Array;
      m_Array = newArray;
    }
  }

  void heapifyDown(int index)
  {
    if (index < 0 || index >= m_Size)
    {
      return;
    }

    if (isALeaf(index))
    {
      return;
    }

    int minChildIndex = getMinChild(index);

    if (heap_type == HEAP_TYPE::MIN_HEAP)
    {
      if (m_Array[index] > m_Array[minChildIndex])
      {
        std::swap(m_Array[index], m_Array[minChildIndex]);
      }
    }
    else
    {
      if (m_Array[index] < m_Array[minChildIndex])
      {
        std::swap(m_Array[index], m_Array[minChildIndex]);
      }
    }

    heapifyDown(minChildIndex);
  }

  bool isALeaf(int index)
  {
    return (getChildIndex(index) >= m_Size);
  }

  bool isRoot(int index)
  {
    return index == 0;
  }

  int getChildIndex(int index)
  {
    return (index+1)*2 - 1;
  }

  int getParentIndex(int index)
  {
    return (index+1)/2 - 1;
  }

  void heapifyUp(int index)
  {
    if (index < 0 || index >= m_Size)
    {
      return;
    }

    if (isRoot(index))
    {
      return;
    }

    int parentIndex = getParentIndex(index);

    if (heap_type == HEAP_TYPE::MIN_HEAP)
    {
      if (m_Array[index] < m_Array[parentIndex])
      {
        std::swap(m_Array[index], m_Array[parentIndex]);
      }
    }
    else
    {
      if (m_Array[index] > m_Array[parentIndex])
      {
        std::swap(m_Array[index], m_Array[parentIndex]);
      }
    }

    heapifyUp(parentIndex);
  }

  int getMinChild(int index)
  {
    if (index < 0 || index >= m_Size)
    {
      return -1;
    }

    int leftChildIndex = getChildIndex(index);
    int rightChildIndex = leftChildIndex + 1;

    if (m_Array[leftChildIndex] < m_Array[rightChildIndex])
    {
      return leftChildIndex;
    }
    else
    {
      return rightChildIndex;
    }
  }

private :

  T*              m_Array;
  int             m_Capacity;
  int             m_Size;
};