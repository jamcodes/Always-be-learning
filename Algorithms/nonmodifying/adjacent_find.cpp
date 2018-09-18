#include <iostream>
#include <array>
#include <algorithm>
#include "auxiliary.h"

/*
template< class ForwardIt >
constexpr ForwardIt adjacent_find( ForwardIt first, ForwardIt last );

template< class ForwardIt, class BinaryPredicate>
constexpr ForwardIt adjacent_find( ForwardIt first, ForwardIt last, BinaryPredicate p );
*/

int main()
{
    std::array<int, 11> coll{4, 3, 2, 1, 23, 23, 16, 17, 81, 9, 14};
    print_elements(coll, "coll: ");

    const auto it1 = std::adjacent_find(coll.cbegin(), coll.cend());
    if (it1 != coll.cend()) {
        std::cout << "first adjacent equal elements are: " << *it1
            << ", at index " << std::distance(coll.cbegin(), it1) << "\n";
    } else {
        std::cout << "adjacent equal elements not found\n";
    }

    const auto it2 = std::adjacent_find(coll.cbegin(), coll.cend(),
                                       [](int frst, int snd){
                                           return frst%3 == 0 && snd%3 == 0;
                                        });
    if (it2 != coll.cend()) {
        std::cout << "first adjacent elements divisible by three are: "
            << *it2 << ", " << *std::next(it2)
            << ", at index " << std::distance(coll.cbegin(), it2) << "\n";
    } else {
        std::cout << "adjacent elements divisible by three not found\n";
    }
}
