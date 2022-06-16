
#define CATCH_CONFIG_MAIN 
#include <catch2/catch.hpp>

#include <string>


#include "avltree.h"

TEST_CASE( "Heap test", "[heap]" ) 
{
    AVLTree<int, std::string> dictionary;
    dictionary.insert(100, "cent");
    dictionary.insert(10, "dix");
    dictionary.insert(1, "un");
    dictionary.insert(5, "cinq");
    dictionary.insert(15, "quinze");
    dictionary.insert(13, "treize");
    
    std::cout<<dictionary<<std::endl;

    dictionary.remove(5);
    std::cout<<"deletion 5 ::"<<std::endl;
    std::cout<<dictionary<<std::endl;

    dictionary.remove(1);
    std::cout<<"deletion 1 ::"<<std::endl;
    std::cout<<dictionary<<std::endl;

    AVLTree<int, std::string> dictionary2 = dictionary;
    std::cout<<"ctor ::"<<std::endl;
    std::cout<<dictionary2<<std::endl;

    dictionary2 = dictionary;
    std::cout<<"operator = ::"<<std::endl;
    std::cout<<dictionary2<<std::endl;

    AVLTree<int, std::string> test 
    {
        std::make_pair(1, "un"), 
        std::make_pair(2, "deux"),
        std::make_pair(3, "trois"),
        std::make_pair(4, "quatre"),
        std::make_pair(5, "cinq")
    };

    std::cout<<"list ctor"<<std::endl;
    std::cout<<test<<std::endl;
}