#include <iostream>
#include <array>
#include <iterator>
#include <algorithm>
#include "auxiliary.h"

/* 
template< class ForwardIt1, class ForwardIt2 >
constexpr bool is_permutation( ForwardIt1 first1, ForwardIt1 last1,
                               ForwardIt2 first2 );

template< class ForwardIt1, class ForwardIt2, class BinaryPredicate >
constexpr bool is_permutation( ForwardIt1 first1, ForwardIt1 last1,
                               ForwardIt2 first2, BinaryPredicate p );

template< class ForwardIt1, class ForwardIt2 >
constexpr bool is_permutation( ForwardIt1 first1, ForwardIt1 last1,
                               ForwardIt2 first2, ForwardIt2 last2 );

template< class ForwardIt1, class ForwardIt2, class BinaryPredicate >
constexpr bool is_permutation( ForwardIt1 first1, ForwardIt1 last1,
                               ForwardIt2 first2, ForwardIt2 last2,
                               BinaryPredicate p );
 */
/**
 * Returns whether the elements in first range are a permutation of the elements in the second.
 * The first overload compares using `==`
 * The second - equality is determined by the BinaryPredicate returning true.
 */


int main()
{
    std::array<int, 5> coll1;
    std::fill_n(coll1.begin(), coll1.size(), rand_gen{0,2});
    std::array<int, 5> coll2;
    std::generate(coll2.begin(), coll2.end(), rand_gen{0,2});
    print_elements(coll1, "coll1: ");
    print_elements(coll2, "coll2: ");

    if (std::is_permutation(coll1.cbegin(), coll1.cend(),
                            coll2.cbegin())) {
        std::cout << "coll1 and coll2 are permutations (have equal elements in different order)\n";
    } else {
        std::cout << "coll1 and coll2 are not permutations\n";
    }

    if (std::is_permutation(coll1.cbegin(), coll1.cend(),
                            coll2.cbegin(), coll2.cend(),
                            [](int f, int s){ return f % 2 == s % 2;})) {
        std::cout << "numbers of even and odd elements match\n";
    } else {
        std::cout << "numbers of even and odd elements don't match\n";
    }
}
