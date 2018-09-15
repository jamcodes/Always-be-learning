#include <iostream>
#include <array>
#include <vector>
#include <iterator>
#include "auxiliary.h"


template< class ForwardIt >
constexpr bool is_sorted( ForwardIt first, ForwardIt last );

template< class ForwardIt, class Compare >
constexpr bool is_sorted( ForwardIt first, ForwardIt last, Compare comp );

template< class ForwardIt >
constexpr ForwardIt is_sorted_until( ForwardIt first, ForwardIt last );

template< class ForwardIt, class Compare >
constexpr ForwardIt is_sorted_until( ForwardIt first, ForwardIt last,
                                     Compare comp );

/*
is_sorted() returns whether the elements in the range [beg,end) are sorted.
is_sorted_until() returns the position of the first element in the range [beg,end), which
The first overload compares using `<`
The second overload compares using binary predicate Compare comp(el1, el2)
*/

int main()
{
    std::array<int, 11> coll1 = { };
    std::generate(coll1.begin(), coll1.end(), rand_gen{0, 9});
    std::sort(coll1.begin(), std::next(coll1.begin(),8));
    print_elements(coll1, "coll1: ");

    if (std::is_sorted(coll1.cbegin(), coll1.cend())) {
        std::cout << "coll1 is sorted" << std::endl;
    } else {
        std::cout << "coll1 is not sorted" << std::endl;
    }

    auto it = std::is_sorted_until(coll1.cbegin(), coll1.cend());
    if (it != coll1.end()) {
        std::cout << "coll1 is sorted until index " << std::distance(coll1.cbegin(), it)
            << ", which is = " << *it << "\n";
    } else {
        std::cout << "coll1 is sorted\n";
    }
}
