#include <iostream>
#include <algorithm>
#include <array>
#include <iterator>
#include <numeric>
#include "auxiliary.h"


template< class InputIt, class OutputIt >
OutputIt partial_sum( InputIt first, InputIt last, OutputIt d_first );

template< class InputIt, class OutputIt, class BinaryOperation >
OutputIt partial_sum( InputIt first, InputIt last, OutputIt d_first,
                      BinaryOperation op );

/*
std::partial_sum
Computes the partial sums of the elements in the subranges of the range [first, last) and writes
them to the range beginning at d_first. The first version uses operator+ to sum up the elements,
the second version uses the given binary function op, both applying std::move to their operands
on the left hand side (since C++20).
Thus, for the values
    a1 a2 a3 ...
they compute either
    a1, a1 + a2, a1 + a2 + a3, ...
or
    a1, a1 op a2, a1 op a2 op a3, ...
Essentially converts relative values to absolute values.
*/


int main()
{
    std::array<int, 6> coll{1,2,3,4,5,6};
    print_elements(coll, "coll: ");

    // print all partial sums
    std::cout << "partial sums: ";
    std::partial_sum(coll.cbegin(), coll.cend(),                    // source range
                     std::ostream_iterator<int>(std::cout, ", "));  // destination
    std::cout << '\n';

    // print all partial products
    std::cout << "partial products: ";
    std::partial_sum(coll.cbegin(), coll.cend(),                    // source range
                     std::ostream_iterator<int>(std::cout, ", "),   // destination
                     [](auto a, auto b){ return a*b; });            // operation
    std::cout << '\n';
}
