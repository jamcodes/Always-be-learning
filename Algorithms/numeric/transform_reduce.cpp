#include <iostream>
#include <algorithm>
#include <numeric>
#include <array>
#include <iterator>
#include "auxiliary.h"


template<class InputIt1, class InputIt2, class T>
T transform_reduce(InputIt1 first1, InputIt1 last1, InputIt2 first2, T init);

template <class InputIt1, class InputIt2, class T, class BinaryOp1, class BinaryOp2>
T transform_reduce(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                   T init, BinaryOp1 binary_op1, BinaryOp2 binary_op2);

template<class InputIt, class T, class BinaryOp, class UnaryOp>
T transform_reduce(InputIt first, InputIt last,
                   T init, BinaryOp binop, UnaryOp unary_op);

/*
1) Equivalent to transform_reduce(first1, last1, first2, init, std::plus<>(), std::multiplies<>()),
effectively parallelized version of the default std::inner_product

2) Applies binary_op2 to each pair of elements from the ranges [first; last) and the range
starting at first2 and reduces the results (possibly permuted and aggregated in unspecified manner)
along with the initial value init over binary_op1

3) Applies unary_op to each element in the range [first; last) and reduces the results
(possibly permuted and aggregated in unspecified manner) along with the initial value init over
binary_op.
*/

int main()
{
    std::array<int, 6> coll{1,2,3,4,5,6};
    print_elements(coll, "coll: ");

    // process sum of all products
    // (0 + 1*1 + 2*2 + 3*3 + 4*4 ... )
    std::cout << "inner product: "
        << std::transform_reduce(coll.cbegin(), coll.cend(),   // first range
                                 coll.cbegin(),                // second range
                                 0)                            // initial value
        << '\n';

    // process product of all sums
    // (1 * 1+1 * 2+2 * 3+3 ...)
    std::cout << "product of sums: "
        << std::transform_reduce(coll.cbegin(), coll.cend(),         // first range
                                 coll.cbegin(),                      // second range
                                 1,                                  // initial value
                                 [](auto a, auto b){ return a*b; },  // outer operation
                                 [](auto a, auto b){ return a+b; })  // inner operation
        << '\n';

    // product of all (values -1)
    std::cout << "product of all values less 1: "
        << std::transform_reduce(coll.cbegin(), coll.cend(),        // first range
                                 coll.cbegin(),                     // second range
                                 1,                                 // init value
                                 [](auto a, auto b){ return a*b; }, // outer operation
                                 [](auto a){ return a-1; })         // inner op
        << '\n';
}
