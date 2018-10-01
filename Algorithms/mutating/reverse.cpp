#include <iostream>
#include <algorithm>
#include <array>
#include <iterator>
#include "auxiliary.h"


template< class BidirIt >
constexpr void reverse( BidirIt first, BidirIt last );

template< class BidirIt, class OutputIt >
constexpr OutputIt reverse_copy( BidirIt first, BidirIt last, OutputIt d_first );

/*
std::reverse
Reverses the order of the elements in the range [first, last)
Behaves as if applying std::iter_swap to every pair of iterators first+i, (last-i) - 1
for each non-negative i < (last-first)/2

std::reverse_copy
Copies the elements from the range [first, last) to another range beginning at d_first
in such a way that the elements in the new range are in reverse order.
Behaves as if by executing the assignment *(d_first + (last - first) - 1 - i) = *(first + i)
once for each non-negative i < (last - first) If the source and destination ranges
(that is, [first, last) and [d_first, d_first+(last-first)) respectively) overlap,
the behavior is undefined.
*/


int main()
{
    std::array<int, 11> coll;
    const auto gen = [i=0u]()mutable noexcept{ return i++; };
    std::generate(coll.begin(), coll.end(), gen);
    print_elements(coll, "coll: ");

    std::reverse(coll.begin(), coll.end());
    print_elements(coll, "coll reversed: ");

    std::reverse(std::next(coll.begin(), 3), std::prev(coll.end(), 3));
    print_elements(coll, "coll partially reversed: ");

    std::cout << "coll printed in reverse order: ";
    std::reverse_copy(coll.cbegin(), coll.cend(), std::ostream_iterator<int>(std::cout, ", "));
}
