#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <iterator>
#include "auxiliary.h"


template< class ForwardIt1, class ForwardIt2 >
constexpr ForwardIt2 swap_ranges( ForwardIt1 first1, ForwardIt1 last1,
                                  ForwardIt2 first2 );

/*
Exchanges elements between range [first1, last1) and another range starting at first2.
Returns the position after the last swapped element in the second range.
*/

int main()
{
    std::array<int, 9> coll1 = {1,2,3,4,5,6,7,8,9};
    std::vector<int> coll2;
    std::generate_n(std::back_inserter(coll2), 9, rand_gen{21, 42});
    print_elements(coll1, "coll1: ");
    print_elements(coll2, "coll2: ");

    auto it = std::swap_ranges(coll1.begin(), coll1. end(),   // first range
                               coll2.begin());                // second range

    if (it != coll2.end()) {
        std::cout << "first not modified element at index " << std::distance(coll2.begin(), it)
            << ", value = " << *it << "\n";
    }

    std::cout << "coll1 and coll2 swapped:\n";
    print_elements(coll1, "\tcoll1: ");
    print_elements(coll2, "\tcoll2: ");

    // swap first three with last three elements of coll2
    std::swap_ranges(coll2.begin(), std::next(coll2.begin(), 3),    // first range
                     coll2.rbegin());                               // second range
    std::cout << "coll2 swapped first three with last three elements:\n";
    print_elements(coll2, "\tcoll2: ");
}
