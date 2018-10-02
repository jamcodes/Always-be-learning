#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <iterator>
#include "auxiliary.h"


template< class BidirIt >
constexpr bool next_permutation( BidirIt first, BidirIt last );

template< class BidirIt, class Compare >
constexpr bool next_permutation( BidirIt first, BidirIt last, Compare comp );

template< class BidirIt >
constexpr bool prev_permutation( BidirIt first, BidirIt last);

template< class BidirIt, class Compare >
constexpr bool prev_permutation( BidirIt first, BidirIt last, Compare comp);

/*
std::next_permutation
Transforms the range [first, last) into the next/previous permutation from the set of all
permutations that are lexicographically ordered with respect to operator< or comp.
Returns true if such permutation exists, otherwise transforms the range into the first/last
permutation (as if by std::sort(first, last) (followed by std::reverse(first, last for prev_)))
and returns false.
*/


int main()
{
    std::array<int,3> coll {1,2,3};
    print_elements(coll, "coll: ");

    // permute elements until they are sorted
    // - runs through all permutations because the elements are sorted now
    while (std::next_permutation(coll.begin(), coll.end())) {
        print_elements(coll, " ");
    }
    print_elements(coll, "after next_permutation cycle: ");

    // permute until descending sorted
    // - this is the next permutation after ascending sorting
    // - so the loop ends immediately
    while (std::prev_permutation(coll.begin(), coll.end())) {
        print_elements(coll, " ");
    }
    print_elements(coll, "after prev_permutation cycle: ");

    // permute elements until they are sorted in descending order
    // - runs through all permutations because the elements are sorted in descending order now
    while (std::prev_permutation(coll.begin(), coll.end())) {
        print_elements(coll, " ");
    }
    print_elements(coll, "after 2nd prev_permutation cycle: ");
}
