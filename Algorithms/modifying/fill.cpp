#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <iterator>
#include "auxiliary.h"


template< class ForwardIt, class T >
constexpr void fill( ForwardIt first, ForwardIt last, const T& value );

template< class OutputIt, class Size, class T >
constexpr OutputIt fill_n( OutputIt first, Size count, const T& value );

/*
std::fill   - Assigns the given value to the elements in the range [first, last)
std::fill_n - Assigns the given value to the first count elements in the range beginning at first,
if count > 0. Does nothing otherwise.
*/


int main()
{
    std::array<int, 9> coll1 = {};
    std::fill(coll1.begin(), coll1.end(), 11);
    print_elements(coll1, "coll1: ");

    std::vector<int> coll2;
    std::fill_n(std::back_inserter(coll2), 9, 42);
    print_elements(coll2, "coll2: ");
}
