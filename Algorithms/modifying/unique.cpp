#include <iostream>
#include <algorithm>
#include <iterator>
#include <array>
#include <vector>
#include "auxiliary.h"


template< class ForwardIt >
constexpr ForwardIt unique( ForwardIt first, ForwardIt last );

template< class ForwardIt, class BinaryPredicate >
constexpr ForwardIt unique( ForwardIt first, ForwardIt last, BinaryPredicate p );

template< class InputIt, class OutputIt >
constexpr OutputIt unique_copy( InputIt first, InputIt last,
                                OutputIt d_first );

template< class InputIt, class OutputIt, class BinaryPredicate >
constexpr OutputIt unique_copy( InputIt first, InputIt last,
                                OutputIt d_first, BinaryPredicate p );

/*
std::unique
Eliminates all but the first element from every group of duplicate (equal) elements
in the range [first, last) and returns a past-the-end iterator for the new logical end of the range

first form:  Elements are compared using operator==
second form: Elements are compared using the given BinaryPredicate p

std::unique_copy
Copies the elements from the range [first, last), to another range beginning at d_first
in such a way that there are no consecutive equal elements.
Only the first element of each group of equal elements is copied.
*/


int main()
{
    std::array<int, 17> source = { 1, 4, 4, 6, 1, 2, 2, 3, 1, 6, 6, 6, 5, 7, 5, 4, 4 };
    std::vector<int> coll;
    std::copy(cbegin(source), cend(source), std::back_inserter(coll));
    print_elements(coll, "coll: ");

    // remove consecutive duplicates
    auto it = std::unique(coll.begin(), coll.end());
    // print elements not removed
    std::cout << "coll, duplicates removed: ";
    std::copy(coll.begin(), it, std::ostream_iterator<int>(std::cout, ", "));
    std::cout << "\n\n";

    // reinitialize
    std::copy(cbegin(source), cend(source), coll.begin());
    print_elements(coll, "coll reinitialized: ");
    // remove elements if there was a previous greater one
    it = std::unique(coll.begin(), coll.end(),
                     [](int a, int b){ return a > b; });
    coll.erase(it, coll.end());
    print_elements(coll, "coll with consecutive lesser elements removed: ");

    std::cout << "\n";
    std::vector<int> coll2;
    // initialize coll2 while removing consecutive duplicates
    std::unique_copy(source.cbegin(), source.cend(),
                     std::back_inserter(coll2));
    print_elements(coll2, "coll2 unique initialized: ");

    std::cout << "coll2 elements without consecutive differing by one: ";
    std::unique_copy(coll2.cbegin(), coll2.cend(),
                     std::ostream_iterator<int>(std::cout, ", "),
                     [](int a, int b){ return a+1 == b || a-1 == b; });
    std::cout << "\n";
}
