#include <iostream>
#include <algorithm>
#include <array>
#include <iterator>
#include "auxiliary.h"


template< class ForwardIt, class T >
constexpr ForwardIt lower_bound( ForwardIt first, ForwardIt last, const T& value );

template< class ForwardIt, class T, class Compare >
constexpr ForwardIt lower_bound( ForwardIt first, ForwardIt last, const T& value, Compare comp );

/*
Returns an iterator pointing to the first element in the range [first, last)
that is not less than (i.e. greater or equal to) value, or last if no such element is found.
This is the first position where an element with value value could get inserted without
breaking the sorting of the range
*/


template< class ForwardIt, class T >
constexpr ForwardIt upper_bound( ForwardIt first, ForwardIt last, const T& value );

template< class ForwardIt, class T, class Compare >
constexpr ForwardIt upper_bound( ForwardIt first, ForwardIt last, const T& value, Compare comp );

/*
Returns an iterator pointing to the first element in the range [first, last)
that is greater than value, or last if no such element is found.
This is the last position where an element with value value could get inserted without breaking the
sorting of the range
*/


template< class ForwardIt, class T >
constexpr std::pair<ForwardIt,ForwardIt>
              equal_range( ForwardIt first, ForwardIt last, const T& value );

template< class ForwardIt, class T, class Compare >
constexpr std::pair<ForwardIt,ForwardIt>
              equal_range( ForwardIt first, ForwardIt last, const T& value, Compare comp );

/*
Returns a range containing all elements equivalent to value in the range [first, last). 
*/


int main()
{
    std::array<int, 16> coll;
    std::iota(coll.begin(), std::next(coll.begin(), 8), 1);
    std::iota(std::next(coll.begin(), 8), coll.end(), 1);
    std::sort(coll.begin(), coll.end());
    print_elements(coll, "coll: ");

    // print first and last position 5 could get inserted
    const auto it1 = std::lower_bound(coll.cbegin(), coll.cend(), 5);
    const auto it2 = std::upper_bound(coll.cbegin(), coll.cend(), 5);

    std::cout << "5 could get index "
        << std::distance(coll.cbegin(), it1)
        << " up to "
        << std::distance(coll.cbegin(), it2)
        << " without breaking the sortin" << '\n';

    auto [it3, it4] = std::equal_range(coll.cbegin(), coll.cend(), 7);
    std::cout << "7 can be found at indices "
        << std::distance(coll.cbegin(), it3)
        << " through "
        << std::distance(coll.cbegin(), it4) << '\n';
}
