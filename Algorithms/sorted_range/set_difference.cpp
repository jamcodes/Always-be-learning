#include <iostream>
#include <array>
#include <algorithm>
#include <iterator>
#include "auxiliary.h"


template< class InputIt1, class InputIt2, class OutputIt >
constexpr OutputIt set_difference( InputIt1 first1, InputIt1 last1,
                                   InputIt2 first2, InputIt2 last2,
                                  OutputIt d_first );

template< class InputIt1, class InputIt2, class OutputIt, class Compare >
constexpr OutputIt set_difference( InputIt1 first1, InputIt1 last1,
                                   InputIt2 first2, InputIt2 last2,
                                   OutputIt d_first, Compare comp );


/*
std::set_difference
Copies the elements from the sorted range [first1, last1) which are not found in the sorted range
[first2, last2) to the range beginning at d_first.
The resulting range is also sorted. Equivalent elements are treated individually, that is,
if some element is found m times in [first1, last1) and n times in [first2, last2),
it will be copied to d_first exactly std::max(m-n, 0) times. The resulting range cannot overlap
with either of the input ranges.
*/


int main()
{
    std::array<int, 5> coll1 = {0,2,3,4,5};
    std::array<int, 5> coll2 = {1,2,3,4,6};
    print_elements(coll1, "coll1: ");
    print_elements(coll2, "coll2: ");

    std::cout << "set_difference: ";
    std::set_difference(coll1.cbegin(), coll1.cend(),
                   coll2.cbegin(), coll2.cend(),
                   std::ostream_iterator<int>(std::cout, ", ")); // 0, 5
    std::cout << '\n';
    std::array<int, 5> coll3 = {0,3,3,4,4};
    std::array<int, 5> coll4 = {3,3,3,4,5};
    print_elements(coll3, "coll3: ");
    print_elements(coll4, "coll4: ");
    std::cout << "set_difference: ";
    std::set_difference(coll3.cbegin(), coll3.cend(),
                   coll4.cbegin(), coll4.cend(),
                   std::ostream_iterator<int>(std::cout, ", "));    // 0, 4
}
