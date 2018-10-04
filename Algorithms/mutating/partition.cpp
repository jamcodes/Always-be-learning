#include <iostream>
#include <array>
#include <iterator>
#include <algorithm>
#include "auxiliary.h"


template< class ForwardIt, class UnaryPredicate >
constexpr ForwardIt partition( ForwardIt first, ForwardIt last, UnaryPredicate p );

template< class BidirIt, class UnaryPredicate >
BidirIt stable_partition( BidirIt first, BidirIt last, UnaryPredicate p );

template< class InputIt, class OutputIt1, class OutputIt2, class UnaryPredicate >
constexpr std::pair<OutputIt1, OutputIt2>
               partition_copy( InputIt first, InputIt last,
                               OutputIt1 d_first_true, OutputIt2 d_first_false,
                               UnaryPredicate p );

/*
std::partition
Reorders the elements in the range [first, last) in such a way that all elements
for which the predicate p returns true precede the elements for which predicate p returns false.
Relative order of the elements is not preserved.
Returns the first position for which the UnaryPredicate returns false (partitioin point).

std::stable_partition
Performs the same operation but preserves the relative order of the elements.

std::partition_copy
Copies the elements from the range [first, last) to two different ranges depending on the value
returned by the predicate p. The elements that satisfy the predicate p are copied to the range
beginning at d_first_true. The rest of the elements are copied to the range beginning at d_first_false.
The behavior is undefined if the input range overlaps either of the output ranges.
*/


int main()
{
    std::vector<int> coll1;
    std::vector<int> coll2;
    const auto gen = [i=0u]()mutable noexcept{return i++;};
    std::generate_n(std::back_inserter(coll1), 10, gen);
    std::generate_n(std::back_inserter(coll2), 10, gen);

    // move all even elements to the front
    const auto pred = [](int elem){ return elem % 2 == 0; };
    const auto it1 = std::partition(coll1.begin(), coll1.end(),         // range
                                    pred);                              // predicate
    const auto it2 = std::stable_partition(coll2.begin(), coll2.end(),  // preserves relative order
                                           pred);                       // of the elements
    print_elements(coll1, "coll1 partitioned: ");
    std::cout << "coll1 partition point = " << *it1 << ", at index "
        << std::distance(coll1.begin(), it1) << "\n";
    print_elements(coll2, "coll2 partitioned: ");
    std::cout << "coll2 partition point = " << *it2 << ", at index "
        << std::distance(coll2.begin(), it2) << "\n";

    coll1.clear();
    coll2.clear();
    std::cout << "\npartition_copy:\n";
    std::array<int, 16> coll3;
    std::generate(coll3.begin(), coll3.end(), rand_gen{0, 42});
    print_elements(coll3, "coll3: ");

    std::partition_copy(coll3.cbegin(), coll3.cend(),
                        std::back_inserter(coll1),      // 'true' elements
                        std::back_inserter(coll2),      // 'false' elements
                        [](int elem){return elem % 2 == 0;});
    print_elements(coll1, "even_elems: ");
    print_elements(coll2, "odd_elems : ");
}
