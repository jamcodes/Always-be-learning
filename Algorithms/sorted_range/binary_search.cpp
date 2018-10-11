#include <iostream>
#include <algorithm>
#include <array>
#include <numeric>
#include "auxiliary.h"


template< class ForwardIt, class T >
constexpr bool binary_search( ForwardIt first, ForwardIt last, const T& value );

template< class ForwardIt, class T, class Compare >
constexpr bool binary_search( ForwardIt first, ForwardIt last, const T& value, Compare comp );

/*
std::binary_search
Checks if an element equivalent to value appears within the range [first, last).
The caller has to ensure that the ranges are sorted according to the sorting criterion on entry
*/


int main()
{
    std::array<int, 10> coll;
    std::iota(coll.begin(), coll.end(), 1);
    print_elements(coll, "coll: ");

    // check for existance of elem with value 5
    if (std::binary_search(coll.cbegin(), coll.cend(), 5)) {
        std::cout << "5 is present\n";
    }
    else {
        std::cout << "5 is not present\n";
    }
}
