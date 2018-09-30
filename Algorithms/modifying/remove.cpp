#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <iterator>
#include "auxiliary.h"


template< class ForwardIt, class T >
constexpr ForwardIt remove( ForwardIt first, ForwardIt last, const T& value );

template< class ForwardIt, class UnaryPredicate >
constexpr ForwardIt remove_if( ForwardIt first, ForwardIt last, UnaryPredicate p );

template< class InputIt, class OutputIt, class T >
constexpr OutputIt remove_copy( InputIt first, InputIt last, OutputIt d_first,
                                const T& value );

template< class InputIt, class OutputIt, class UnaryPredicate >
constexpr OutputIt remove_copy_if( InputIt first, InputIt last, OutputIt d_first,
                                   UnaryPredicate p );

/*
std::remove
Removes all elements satisfying specific criteria from the range [first, last)
and returns a past-the-end iterator for the new logical end of the range.
The container containing the range isn't itself mutated.

std::remove_copy
Copies elements from the range [first, last), to another range beginning at d_first,
omitting the elements which satisfy specific criteria.
*/


int main()
{
    std::vector<int> coll;
    std::generate_n(std::back_inserter(coll), 11, rand_gen{0, 10});
    coll.front() = 5;
    coll.back() = 5;
    std::cout << "coll size = " << coll.size() << "\n";
    print_elements(coll, "coll: ");

    auto it = std::remove(coll.begin(), coll.end(),     // range
                          5);                           // value to remove
    std::cout << "coll size after remove = " << coll.size() << "\n";    // size not changed
    print_elements(coll, "coll after remove, before erase: ");

    coll.erase(it, coll.end());
    std::cout << "coll size after erase = " << coll.size() << "\n";
    print_elements(coll, "coll after remove and erase: ");

    it = std::remove_if(coll.begin(), coll.end(),       // range
                        [](int i){ return i < 5;});     // removal criteria
    coll.erase(it, coll.end());
    print_elements(coll, "coll with elements < 5 removed: ");

    std::vector<int> coll2;
    std::remove_copy_if(coll.cbegin(), coll.cend(),     // source range
                        std::back_inserter(coll2),      // destination
                        [](int i){ return i%2; });      // predicate
    print_elements(coll2, "coll2 - odd elements removed: ");
}
