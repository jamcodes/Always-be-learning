#include <iostream>
#include <algorithm>
#include <array>
#include <iterator>
#include "auxiliary.h"


template< class InputIt, class OutputIt >
constexpr OutputIt adjacent_difference( InputIt first, InputIt last,
                                        OutputIt d_first );

template< class InputIt, class OutputIt, class BinaryOperation >
constexpr OutputIt adjacent_difference( InputIt first, InputIt last,
                                        OutputIt d_first, BinaryOperation op );

/*
std::adjacent_difference
Computes the differences between the second and the first of each adjacent pair of elements
of the range [first, last) and writes them to the range beginning at d_first + 1.
An unmodified copy of *first is written to *d_first.
Thus, for the values
    a1 a2 a3 a4 ...
they compute and write either the values
    a1, a2 - a1, a3 - a2, a4 - a3, ...
or the values
    a1, a2 op a1, a3 op a2, a4 op a3, ...
respectively.
*/


int main()
{
    std::array<int, 6> coll{1,2,3,4,5,6};

    // print all differences between elements
    std::cout << "differences between elements: ";
    std::adjacent_difference(coll.cbegin(), coll.cend(),                    // source
                             std::ostream_iterator<int>(std::cout, ", "));  // destination
    std::cout << '\n';

    // print all sums with the predecessors
    std::cout << "sums between elements: ";
    std::adjacent_difference(coll.cbegin(), coll.cend(),                    // source
                             std::ostream_iterator<int>(std::cout, ", "),   // destination
                             [](auto a, auto b){ return a+b; });            // operation
    std::cout << '\n';

    // products between elements
    std::cout << "products between elements: ";
    std::adjacent_difference(coll.cbegin(), coll.cend(),                    // source
                             std::ostream_iterator<int>(std::cout, ", "),   // destination
                             [](auto a, auto b){ return a*b; });            // operation
    std::cout << '\n';
}
