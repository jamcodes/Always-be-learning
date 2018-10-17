#include <iostream>
#include <array>
#include <algorithm>
#include <iterator>
#include "auxiliary.h"


template< class InputIt1, class InputIt2, class OutputIt >
constexpr OutputIt set_intersection( InputIt1 first1, InputIt1 last1,
                                     InputIt2 first2, InputIt2 last2,
                                     OutputIt d_first );

template< class InputIt1, class InputIt2, class OutputIt, class Compare >
constexpr OutputIt set_intersection( InputIt1 first1, InputIt1 last1,
                                     InputIt2 first2, InputIt2 last2,
                                     OutputIt d_first, Compare comp );

/*
std::set_intersection
Constructs a sorted range beginning at d_first consisting of elements that are found in both
sorted ranges [first1, last1) and [first2, last2). If some element is found m times in
[first1, last1) and n times in [first2, last2), the first std::min(m, n) elements will be copied
from the first range to the destination range. The order of equivalent elements is preserved.
The resulting range cannot overlap with either of the input ranges.
*/

int main()
{
    std::array<int, 5> coll1 = {1,2,3,3,4};
    std::array<int, 5> coll2 = {0,1,2,3,4};
    print_elements(coll1, "coll1: ");
    print_elements(coll2, "coll2: ");

    std::cout << "set_intersection: ";
    std::set_intersection(coll1.cbegin(), coll1.cend(),
                   coll2.cbegin(), coll2.cend(),
                   std::ostream_iterator<int>(std::cout, ", ")); // 1, 2, 3, 4,

    std::cout << '\n';
    std::fill(std::next(coll2.begin(), 1), coll2.end(), 3);
    print_elements(coll1, "coll1: ");
    print_elements(coll2, "coll2: ");
    std::cout << "set_intersection: ";
    std::set_intersection(coll1.cbegin(), coll1.cend(),
                   coll2.cbegin(), coll2.cend(),
                   std::ostream_iterator<int>(std::cout, ", "));    // 3, 3
}
