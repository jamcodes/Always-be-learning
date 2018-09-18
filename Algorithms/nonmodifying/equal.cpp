#include <iostream>
#include <array>
#include <algorithm>
#include <iterator>
#include "auxiliary.h"


/* template< class InputIt1, class InputIt2 >
constexpr bool equal( InputIt1 first1, InputIt1 last1,
                      InputIt2 first2 );

template< class InputIt1, class InputIt2, class BinaryPredicate >
constexpr bool equal( InputIt1 first1, InputIt1 last1,
                      InputIt2 first2, BinaryPredicate p );

template< class InputIt1, class InputIt2 >
bool equal( InputIt1 first1, InputIt1 last1,
            InputIt2 first2, InputIt2 last2 );

template< class InputIt1, class InputIt2, class BinaryPredicate >
constexpr bool equal( InputIt1 first1, InputIt1 last1,
                      InputIt2 first2, InputIt2 last2,
                      BinaryPredicate p ); */
/**
 * Returns whether the elements in the range [first1, last1) are equal to the elements
 * in the range starting with first2, or for the range [first2, last2).
 * For the BinaryPredicate overload - whether the predicate returns true for the range.
 */



int main()
{
    std::array<int, 11> coll1;
    std::fill(coll1.begin(), coll1.end(), rand_gen{0, 10});
    std::vector<int> coll2;
    std::generate_n(std::back_inserter(coll2), 11, rand_gen{0, 10});
    print_elements(coll1, "coll1: ");
    print_elements(coll2, "coll2: ");

    if (std::equal(coll1.cbegin(), coll1.cend(),
                   coll2.cbegin()))
    {
        std::cout << "elements of coll1 are all equal to elements of coll2\n";
    } else {
        std::cout << "elements of coll1 are not equal to elements of coll2\n";
    }

    const auto pred = [](int lhs, int rhs){ return lhs%2 == rhs%2; };
    if (std::equal(coll1.cbegin(), coll1.cend(),
                   coll2.cbegin(),
                   pred))
    {
        std::cout << "corresponding elements of both ranges meet the predicate's requirements\n";
    } else {
        std::cout << "corresponding elements don't meet the predicate\n";
    }
}
