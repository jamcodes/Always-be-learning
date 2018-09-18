#include <iostream>
#include <algorithm>
#include <array>
#include <iterator>
#include "auxiliary.h"


template< class InputIt, class OutputIt, class UnaryOperation >
constexpr OutputIt transform( InputIt first1, InputIt last1, OutputIt d_first,
                              UnaryOperation unary_op );

template< class InputIt1, class InputIt2, class OutputIt, class BinaryOperation >
constexpr OutputIt transform( InputIt1 first1, InputIt1 last1, InputIt2 first2,
                              OutputIt d_first, BinaryOperation binary_op );

/*
std::transform applies the given function to a range and stores the result in another range,
               beginning at d_first.
- The unary operation unary_op is applied to the range defined by [first1, last1).
- The binary operation binary_op is applied to pairs of elements from two ranges:
    one defined by [first1, last1) and the other beginning at first2.
*/

int main()
{
    std::array<int, 9> coll1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    print_elements(coll1, "coll1: ");
    std::transform(coll1.cbegin(), coll1.cend(),    // source range
                   coll1.begin(),                   // destination range
                   [](int i){ return -i; });        // operation
    print_elements(coll1, "coll1 negated: ");

    std::vector<int> coll2;
    std::transform(coll1.cbegin(), coll1.cend(),    // source
                   back_inserter(coll2),            // destination
                   [](int i){ return i*10; });      // operation
    print_elements(coll2, "coll2: ");
    
    std::cout << "transform and output coll1 and coll2:\n";
    std::transform(coll1.cbegin(), coll1.cend(),                // source 1
                   coll2.crbegin(),                             // begining of source 2
                   std::ostream_iterator<int>(std::cout, ", "), // destination
                   [](int el1, int el2) { return el1 * el2;});  // operation

}
