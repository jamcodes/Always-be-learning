#include <iostream>
#include <array>
#include <vector>
#include <iterator>
#include "auxiliary.h"


template< class InputIt1, class InputIt2, class OutputIt >
constexpr OutputIt set_union( InputIt1 first1, InputIt1 last1,
                              InputIt2 first2, InputIt2 last2,
                              OutputIt d_first );

template< class InputIt1, class InputIt2, class OutputIt, class Compare >
constexpr OutputIt set_union( InputIt1 first1, InputIt1 last1,
                              InputIt2 first2, InputIt2 last2,
                              OutputIt d_first, Compare comp );

/*
Constructs a sorted union beginning at d_first consisting of the set of elements present
in one or both sorted ranges [first1, last1) and [first2, last2).
Elements found in both ranges are in the union range only once. However if there are multiple
occurences of an element in a single range then multiple occurences are in the union. The number
of occurences in the union is the maximum of occurences in range1 and range2.
Both forms return the position after the last copied element in the destination range (the first
element that is not overwritten).
*/


int main()
{
    std::array<int, 5> coll1 = {1,2,3,4,5};
    std::array<int, 5> coll2 = {3,4,5,6,7};
    print_elements(coll1, "coll1: ");
    print_elements(coll2, "coll2: ");

    std::cout << "set_union: ";
    std::set_union(coll1.cbegin(), coll1.cend(),
                   coll2.cbegin(), coll2.cend(),
                   std::ostream_iterator<int>(std::cout, ", ")); // 1, 2, 3, 4, 5, 6, 7

    std::cout << '\n';
    std::fill(std::next(coll1.begin(), 1), coll1.end(), 5);
    print_elements(coll1, "coll1: ");
    print_elements(coll2, "coll2: ");
    std::cout << "set_union: ";
    std::set_union(coll1.cbegin(), coll1.cend(),
                   coll2.cbegin(), coll2.cend(),
                   std::ostream_iterator<int>(std::cout, ", "));    // 1, 3, 4, 5, 5, 5, 5, 6, 7
}
