#include <iostream>
#include <array>
#include <algorithm>
#include <random>
#include "auxiliary.h"


template< class RandomIt, class URBG >
void shuffle( RandomIt first, RandomIt last, URBG&& g );

/*
Reorders the elements in the given range [first, last)
such that each possible permutation of those elements has equal probability of appearance. 
Uses a random-number generator engine.
*/


int main()
{
    std::array<int, 11> coll{0,1,2,3,4,5,6,7,8,9,10};
    print_elements(coll, "coll: ");

    std::default_random_engine dre;
    std::shuffle(coll.begin(), coll.end(),  // range
                 dre);                      // random-number generator

    print_elements(coll, "shuffled: ");
}
