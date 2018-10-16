#include <iostream>
#include <array>
#include <vector>
#include <iterator>
#include "auxiliary.h"


template< class InputIt1, class InputIt2, class OutputIt >
constexpr OutputIt merge( InputIt1 first1, InputIt1 last1,
                          InputIt2 first2, InputIt2 last2,
                          OutputIt d_first );

template< class InputIt1, class InputIt2, class OutputIt, class Compare>
constexpr OutputIt merge( InputIt1 first1, InputIt1 last1,
                          InputIt2 first2, InputIt2 last2,
                          OutputIt d_first, Compare comp );

/*
Merges two sorted ranges [first1, last1) and [first2, last2) into one sorted range
beginning at d_first. The caller has to ensure that both source ranges are sorted on entry.
Both forms return the position after the last copied element in the destination range (the first
element that is not overwritten).
*/


int main()
{
    std::array<int, 9> coll1;
    std::array<int, 6> coll2;
    std::iota(coll1.begin(), coll1.end(), 1);
    std::iota(coll2.begin(), coll2.end(), 3);
    print_elements(coll1, "coll1: ");
    print_elements(coll2, "coll2: ");

    std::cout << "merged: ";
    std::merge(coll1.cbegin(), coll1.cend(),
               coll2.cbegin(), coll2.cend(),
               std::ostream_iterator<int>(std::cout, ", "));
    std::cout << '\n';
}
