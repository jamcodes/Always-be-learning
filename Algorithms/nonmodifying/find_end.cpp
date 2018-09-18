#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <iterator>
#include "auxiliary.h"

/*
std::find_end - searches for the last occurence of subrange [s_first, s_last)
in range [first, last).

template< class ForwardIt1, class ForwardIt2 >
constexpr ForwardIt1 find_end( ForwardIt1 first, ForwardIt1 last,
                               ForwardIt2 s_first, ForwardIt2 s_last );

template< class ForwardIt1, class ForwardIt2, class BinaryPredicate >
constexpr ForwardIt1 find_end( ForwardIt1 first, ForwardIt1 last,
                               ForwardIt2 s_first, ForwardIt2 s_last, BinaryPredicate p );
*/


int main()
{
    std::vector<int> coll;
    std::array<int, 4> subcoll{3,4,5,6};
    const auto gen = [i=0]()mutable noexcept{return i++;};
    std::generate_n(std::back_inserter(coll), 7, gen);
    std::generate_n(std::back_inserter(coll), 7, gen);
    print_elements(coll, "coll: ");
    print_elements(subcoll, "subcoll: ");

    const auto it1 = std::find_end(coll.cbegin(), coll.cend(),          // range
                                   subcoll.cbegin(), subcoll.cend());   // subrange
    if (it1 != coll.cend()) {
        std::cout << "last subcoll found starting at index "
            << std::distance(coll.cbegin(), it1) << "\n";
    } else {
        std::cout << "subcoll not found\n";
    }

    const auto it2 = std::find_end(coll.cbegin(), coll.cend(),          // range
                                   subcoll.cbegin(), subcoll.cend(),    // subrange
                                   [](int el, int subel)noexcept{       // binary predicate
                                       return el == --subel;}
                                    );
    if (it2 != coll.cend()) {
        std::cout << "last subcoll meeting the binary predicate requirements\nstarts at index: "
            << std::distance(coll.cbegin(), it2) << "\n";
    } else {
        std::cout << "subcoll meeting the binary predicate not found\n";
    }
}
