#include <iostream>
#include <algorithm>
#include <array>
#include <iterator>
#include <numeric>
#include "auxiliary.h"


template< class InputIt, class T >
T accumulate( InputIt first, InputIt last, T init );

template< class InputIt, class T, class BinaryOperation >
T accumulate( InputIt first, InputIt last, T init, BinaryOperation op );

/*
std::accumulate
Computes the sum of the given value init and the elements in the range [first, last).
The first version uses operator+ to sum up the elements, the second version uses the given
binary function op, both applying std::move to their operands on the left hand side (since C++20).
*/


int main()
{
    std::array<int, 9> coll{};
    std::iota(coll.begin(), coll.end(), 1);
    print_elements(coll, "coll: ");

    // process sum of elements
    std::cout << "sum: "
        << std::accumulate(coll.cbegin(), coll.cend(),      // range
                           0)                               // initial value
        << '\n';

    // process product of elements
    std::cout << "product: "
        << std::accumulate(coll.cbegin(), coll.cend(),          // range
                           1,                                   // initial value
                           [](int a, int b){ return a * b; })   // operation
        << '\n';
}
