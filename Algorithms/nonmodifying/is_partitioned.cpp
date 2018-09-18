#include <iostream>
#include <array>
#include <iterator>
#include "auxiliary.h"


template< class InputIt, class UnaryPredicate >
constexpr bool is_partitioned( InputIt first, InputIt last, UnaryPredicate p );

template< class ForwardIt, class UnaryPredicate >
constexpr ForwardIt partition_point( ForwardIt first, ForwardIt last, UnaryPredicate p );

/*
is_partitioned() returns whether the elements in the range [first,last) are partitions, so all the
elements fulfilling the predicate op() are positioned before all elements that do not fulfill it.
partition_point() returns the position of the first element in the partitioned range [beg,end)
*/

int main()
{
    std::array<int, 9> coll = { 5, 3, 9, 1, 3, 4, 8, 2, 6 };
    print_elements(coll, "coll: ");

    const auto isOdd = [](int elem) { return elem%2 == 1; };

    if (std::is_partitioned(coll.cbegin(), coll.cend(), // range
                            isOdd)) {                   // predicate
        std::cout << "coll is partitioned\n";

        // find first even element
        auto pos = std::partition_point(coll.cbegin(), coll.cend(), isOdd);
        std::cout << "first even element: " << *pos << "\n";
    } else {
        std::cout << "coll is not partitioned\n";
    }
}
