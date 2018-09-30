#include <iostream>
#include <algorithm>
#include <array>
#include <iterator>
#include "auxiliary.h"


template< class ForwardIt, class T >
constexpr void replace( ForwardIt first, ForwardIt last,
                        const T& old_value, const T& new_value );

template< class ForwardIt, class UnaryPredicate, class T >
constexpr void replace_if( ForwardIt first, ForwardIt last,
                           UnaryPredicate p, const T& new_value );

template< class InputIt, class OutputIt, class T >
constexpr OutputIt replace_copy( InputIt first, InputIt last, OutputIt d_first,
                                 const T& old_value, const T& new_value );

template< class InputIt, class OutputIt, class UnaryPredicate, class T >
constexpr OutputIt replace_copy_if( InputIt first, InputIt last, OutputIt d_first,
                                    UnaryPredicate p, const T& new_value );

/*
Replaces all elements satisfying specific criteria with new_value in the range [first, last)
std::replace    - replaces all elements that are equal to old_value.
std::replace_if - replaces all elements for which predicate p returns true.

std::replace_copy
Copies the all elements from the range [first, last) to another range beginning at d_first
replacing all elements satisfying specific criteria with new_value.
The source and destination ranges cannot overlap.
*/


int main()
{
    std::array<int, 11> coll;
    std::generate(coll.begin(), coll.end(), rand_gen{0, 49});
    coll.front() = 6;
    coll[5] = 6;
    coll.back() = 6;
    print_elements(coll, "coll: ");

    std::replace(coll.begin(), coll.end(),              // range
                 6,                                     // old value
                 42);                                   // new value
    print_elements(coll, "coll after replace: ");

    std::replace_if(coll.begin(), coll.end(),           // range
                    [](int i){return i < 42;},          // predicate
                    0);                                 // new value
    print_elements(coll, "coll after replace_if: ");

    std::vector<int> coll2;
    std::replace_copy_if(coll.cbegin(), coll.cend(),    // source range
                         std::back_inserter(coll2),     // destination
                         [](int i){return i >= 42;},    // predicate
                         144);                          // new value
    print_elements(coll2, "coll2: ");
}
