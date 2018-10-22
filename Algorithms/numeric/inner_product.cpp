#include <iostream>
#include <algorithm>
#include <array>
#include <iterator>
#include <numeric>
#include "auxiliary.h"


template< class InputIt1, class InputIt2, class T >
T inner_product( InputIt1 first1, InputIt1 last1,
                 InputIt2 first2, T init );

template<class InputIt1, class InputIt2, class T,
         class BinaryOperation1, class BinaryOperation2>
T inner_product( InputIt1 first1, InputIt1 last1,
                 InputIt2 first2, T init,
                 BinaryOperation1 op1,
                 BinaryOperation2 op2 );

/*
std::inner_product
Computes inner product (i.e. sum of products) or performs ordered map/reduce operation
on the range [first1, last1) and the range beginning at first2.
first form   ->  initValue = std::move(init) + elem1 * elem2 ...
second form  ->  initValue = op1(std::move(init),op2(elem1,elem2)) ...
*/

int main()
{
    std::array<int, 6> coll{1,2,3,4,5,6};
    print_elements(coll, "coll: ");

    // process sum of all products
    // (0 + 1*1 + 2*2 + 3*3 + 4*4 ... )
    std::cout << "inner product: "
        << std::inner_product(coll.cbegin(), coll.cend(),   // first range
                              coll.cbegin(),                // second range
                              0)                            // initial value
        << '\n';

    // process sum of (1*6 + 2*5 ... 6*1)
    std::cout << "inner reverse product: "
        << std::inner_product(coll.cbegin(), coll.cend(),   // first range
                              coll.crbegin(),               // second range
                              0)                            // initial value
        << '\n';

    // process product of all sums
    // (1 * 1+1 * 2+2 * 3+3 ...)
    std::cout << "product of sums: "
        << std::inner_product(coll.cbegin(), coll.cend(),         // first range
                              coll.cbegin(),                      // second range
                              1,                                  // initial value
                              [](auto a, auto b){ return a*b; },  // outer operation
                              [](auto a, auto b){ return a+b; })  // inner operation
        << '\n';
}
