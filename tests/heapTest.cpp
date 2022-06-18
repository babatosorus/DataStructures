
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <string>

#include "heap.h"


TEST_CASE( "MinHeap test",)
{

  SECTION("insert/remove tests")
  {
    MinHeap<int> heap;

    REQUIRE(heap.capacity() == 0);
    heap.insert(11);
    REQUIRE(heap.capacity() == 1);
    heap.insert(3);
    REQUIRE(heap.capacity() == 2);
    heap.insert(10);
    heap.insert(5);
    REQUIRE(heap.capacity() == 4);
    heap.insert(2);
    REQUIRE(heap.capacity() == 8);
    heap.insert(101);

    REQUIRE(heap.size() == 6);

    const std::vector<int> expectedValues = {2,3, 5, 10,11,101};

    for (auto val : expectedValues)
    {
      REQUIRE(val == heap.remove());
    }

    REQUIRE_THROWS(heap.remove());
  }

  SECTION("clear")
  {
    MinHeap<int> heap;
    heap.insert(11);
    heap.insert(3);

    heap.clear();
    REQUIRE(heap.capacity() == 0);
    REQUIRE(heap.size() == 0);

  }
}
