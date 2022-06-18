
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <string>
#include <queue>

#include "avltree.h"

TEST_CASE( "AVL Tree" )
{
  AVLTree<int, std::string> dictionary;

  dictionary.insert(100, "cent");
  dictionary.insert(10, "dix");
  dictionary.insert(1, "un");
  dictionary.insert(5, "cinq");
  dictionary.insert(15, "quinze");
  dictionary.insert(13, "treize");

  REQUIRE(dictionary.size() == 6);
  REQUIRE(dictionary.height() == 2);

  std::queue<int> s({1, 5, 10, 13, 15, 100});
  auto f = [&](const int& key, const std::string& val)
  {
    REQUIRE(s.front() == key);
    s.pop();
  };
  dictionary.inOrderTraverse(f);

  dictionary.remove(5);
  dictionary.remove(1);
  dictionary.insert(122, "cent vingt deux");
  dictionary.insert(0, "zero");
  REQUIRE(dictionary.size() == 6);
  REQUIRE(dictionary.height() == 2);

  s =  std::queue<int>({0, 10, 13, 15, 100, 122});
  dictionary.inOrderTraverse(f);

}