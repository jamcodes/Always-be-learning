#include <iostream>
#include <algorithm>
#include <array>
#include <iterator>
#include "auxiliary.h"


template< class RandomIt >
constexpr bool is_heap( RandomIt first, RandomIt last );

template< class RandomIt, class Compare >
constexpr bool is_heap( RandomIt first, RandomIt last, Compare comp );

template< class RandomIt >
constexpr RandomIt is_heap_until( RandomIt first, RandomIt last );

template< class RandomIt, class Compare >
constexpr RandomIt is_heap_until( RandomIt first, RandomIt last, Compare comp );

/*
is_heap() returns whether the elements in the range [first,last) are a heap,
which means that first is (one of) the maximum element(s).
is_heap()_until returns the position of the first element in the range [first,last) that breaks the
sorting as a heap (is larger than the first element) or end if none.
The first and third forms use operator < to compare elements. The second and fourth forms use
the binary predicate Compare comp(el1,el2) which should return true if el1 is “less than” el2.
*/

int main()
{
    std::array<int, 9> coll1 = { 9, 8, 7, 7, 7, 5, 4, 2, 1 };
    std::array<int, 9> coll2 = { 5, 3, 2, 1, 4, 7, 9, 8, 6 };
    print_elements(coll1, "coll1: ");
    print_elements(coll2, "coll2: ");

    std::cout << std::boolalpha << "coll1 is heap: "
        << std::is_heap(coll1.cbegin(), coll1.cend()) << std::endl;
    std::cout << "coll2 is heap: "
        << std::is_heap(coll2.cbegin(), coll2.cend()) << std::endl;

    const auto it = std::is_heap_until(coll2.cbegin(), coll2.cend());
    if (it != coll2.cend()) {
        std::cout << "coll2 is a heap until index " << std::distance(coll2.cbegin(), it)
            << ", which equals " << *it << "\n";
    } else {
        std::cout << "coll2 is a heap\n";
    }
}
