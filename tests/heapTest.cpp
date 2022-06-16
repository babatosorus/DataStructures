
#define CATCH_CONFIG_MAIN 
#include <catch2/catch.hpp>

#include <string>

#include "heap.h"


TEST_CASE( "Heap test", "[heap]" ) 
{
	Heap<int, HEAP_TYPE::MIN_HEAP> heap;
    
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

    const int* test = static_cast<const int*>(heap);
    std::vector<int> expectedValues = {2,3,10,11,5,101};

    for (int i = 0 ; i < heap.size(); ++i)
    	REQUIRE(expectedValues[i] == test[i]);

    int val = heap.remove();
    REQUIRE(val == 2);

    val = heap.remove();
    REQUIRE(val == 3);

    test = static_cast<const int*>(heap);
    expectedValues = {5,11,10,101};
    for (int i = 0 ; i < 4; ++i)
    	REQUIRE(expectedValues[i] == test[i]);

    val = heap.remove();
    REQUIRE(val == 5);
}
