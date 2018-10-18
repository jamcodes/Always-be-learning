#include <iostream>
#include <algorithm>
#include <array>
#include <iterator>
#include "auxiliary.h"


template< class InputIt1, class InputIt2, class OutputIt >
constexpr OutputIt set_symmetric_difference( InputIt1 first1, InputIt1 last1,
                                             InputIt2 first2, InputIt2 last2,
                                             OutputIt d_first );

template< class InputIt1, class InputIt2, class OutputIt, class Compare >
constexpr OutputIt set_symmetric_difference( InputIt1 first1, InputIt1 last1,
                                             InputIt2 first2, InputIt2 last2,
                                             OutputIt d_first, Compare comp );

/*
std::set_symmetric_difference
Computes symmetric difference of two sorted ranges: the elements that are found in either of the
ranges, but not in both of them are copied to the range beginning at d_first.
The resulting rangeis also sorted.
If some element is found m times in [first1, last1) and n times in [first2, last2),
it will be copied to d_first exactly std::abs(m-n) times. If m>n, then the last m-n of those
elements are copied from [first1,last1), otherwise the last n-m elements are copied from
[first2,last2). The resulting range cannot overlap with either of the input ranges.
*/


int main()
{
    std::array<int, 5> coll1 = {0,2,3,4,5};
    std::array<int, 5> coll2 = {1,2,3,4,6};
    print_elements(coll1, "coll1: ");
    print_elements(coll2, "coll2: ");

    std::cout << "set_symmetric_difference: ";
    std::set_symmetric_difference(coll1.cbegin(), coll1.cend(),
                   coll2.cbegin(), coll2.cend(),
                   std::ostream_iterator<int>(std::cout, ", ")); // 0, 5
    std::cout << '\n';
    std::array<int, 5> coll3 = {0,3,3,4,4};
    std::array<int, 5> coll4 = {3,3,3,4,5};
    print_elements(coll3, "coll3: ");
    print_elements(coll4, "coll4: ");
    std::cout << "set_symmetric_difference: ";
    std::set_symmetric_difference(coll3.cbegin(), coll3.cend(),
                   coll4.cbegin(), coll4.cend(),
                   std::ostream_iterator<int>(std::cout, ", "));    // 0, 4
}
