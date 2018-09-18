#include <iostream>
#include <algorithm>
#include <array>
#include <tuple>
#include "auxiliary.h"

template< class InputIt1, class InputIt2 >
constexpr std::pair<InputIt1,InputIt2> mismatch( InputIt1 first1, InputIt1 last1,
                                                 InputIt2 first2 );

template< class InputIt1, class InputIt2, class BinaryPredicate >
constexpr std::pair<InputIt1,InputIt2> mismatch( InputIt1 first1, InputIt1 last1,
                                                 InputIt2 first2,
                                                 BinaryPredicate p );

template< class InputIt1, class InputIt2 >
constexpr std::pair<InputIt1,InputIt2> mismatch( InputIt1 first1, InputIt1 last1,
                                                 InputIt2 first2, InputIt2 last2 );

template< class InputIt1, class InputIt2, class BinaryPredicate >
constexpr std::pair<InputIt1,InputIt2> mismatch( InputIt1 first1, InputIt1 last1,
                                                 InputIt2 first2, InputIt2 last2,
                                                 BinaryPredicate p );

/**
 * Returns the first two corresponding elements of the given ranges that differ.
 * BinaryPredicate overload returns the first pair for which false is returned
 */

int main()
{
    std::array<int, 6> coll1 = {1, 2, 3, 4, 5, 6};
    std::array<int, 6> coll2 = {1, 2, 4, 8, 16, 3};
    print_elements(coll1, "coll1: ");
    print_elements(coll2, "coll2: ");

    // find first mismatch
    auto [fst, snd] = std::mismatch(coll1.cbegin(), coll1.cend(),   // first range
                                    coll2.cbegin());                // second range
    if (fst == coll1.end()) {
        std::cout << "no mismatch found\n";
    } else {
        std::cout << "first mismatch: " << *fst << " and " << *snd
            << " at index: " << std::distance(coll1.cbegin(), fst) << "\n";
    }

    // find the first position where the element of coll1 is not
    // less than the corresponding element of coll2
    std::tie(fst, snd) = std::mismatch(coll1.cbegin(), coll1.cend(),
                               coll2.cbegin(), coll2.cend(),
                               [](int f, int s){ return f <= s; });
    if (fst == coll1.end()) {
        std::cout << "always less or equal\n";
    } else {
        std::cout << "first not less or equal: " << *fst << " and " << *snd
            << " at index: " << std::distance(coll1.cbegin(), fst) << "\n";
    }
}
