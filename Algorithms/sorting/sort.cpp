#include <iostream>
#include <algorithm>
#include <numeric>
#include <array>
#include <iterator>
#include <vector>
#include "auxiliary.h"


template< class RandomIt >
constexpr void sort( RandomIt first, RandomIt last );

template< class RandomIt, class Compare >
constexpr void sort( RandomIt first, RandomIt last, Compare comp );

template< class RandomIt >
void stable_sort( RandomIt first, RandomIt last );

template< class RandomIt, class Compare >
void stable_sort( RandomIt first, RandomIt last, Compare comp );

/*
std::sort
Sorts the elements in the range [first, last) in ascending order.
The order of equal elements is not guaranteed to be preserved.

std::stable_sort
Sorts the elements in the range [first, last) in ascending order.
The order of equivalent elements is guaranteed to be preserved.
*/

int main()
{
    std::array<int, 16> coll;
    std::iota(coll.begin(), std::next(coll.begin(), 8), 1);
    std::generate(std::next(coll.begin(), 8), coll.end(), rand_gen{0,11});
    print_elements(coll, "coll: ");

    std::sort(coll.begin(), coll.end());
    print_elements(coll, "sorted: ");

    std::sort(coll.begin(), coll.end(),             // range
              [](auto a, auto b){ return b < a; }); // predicate - here sort in reverse
    print_elements(coll, "sorted in reverse: ");
}



// Provide the template definition in a header (t.hpp), then suppress automatic instantiation
// for commonly used specializations by declaring them extern,
// and explicitly insantiate them in a source file (t.cpp)

// --- t.hpp:
template<typename T>
void f()
{
    // ... definition
}

extern template void f<int>();      // declared but not defined
extern template void f<float>();

// --- t.cpp:
template void f<int>();             // definition
template void f<float>();           // definition
