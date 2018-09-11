#include <iostream>
#include <algorithm>
#include <array>
#include <iterator>
#include "auxiliary.h"

/*
template< class InputIt, class ForwardIt >
constexpr InputIt find_first_of( InputIt first, InputIt last,
                                 ForwardIt s_first, ForwardIt s_last );

template< class InputIt, class ForwardIt, class BinaryPredicate >
constexpr InputIt find_first_of( InputIt first, InputIt last,
                                 ForwardIt s_first, ForwardIt s_last, BinaryPredicate p );
*/

int main()
{
    std::vector<int> coll;
    std::generate_n(std::back_inserter(coll), 11, rand_gen{0, 50});
    std::array<int, 5> searchcoll{4, 9, 11, 42, 49};
    print_elements(coll, "coll: ");
    print_elements(searchcoll, "searchcoll: ");

    const auto it1 = std::find_first_of(coll.cbegin(), coll.cend(),     // range
                                        searchcoll.cbegin(),            // beginning of search set
                                        searchcoll.cend());             // end of search set
    if (it1 != coll.cend()) {
        std::cout << "first element of searchcoll that's in coll is: "
            << *it1 << ", at index " << std::distance(coll.cbegin(), it1) << "\n";
    } else {
        std::cout << "none of the elements in searchcoll found in coll\n";
    }

    const auto it2 = std::find_first_of(coll.cbegin(), coll.cend(),                 // range
                                        searchcoll.cbegin(), searchcoll.cend(),     // search set
                                        [](int el, int s){ return el % s == 0;});   // predicate
    if (it2 != coll.cend()) {
        std::cout << "first element of coll that's divisible by one of searchcoll elements is "
            << *it2 << ", at index " << std::distance(coll.cbegin(), it2) << "\n";
    } else {
        std::cout << "none of the element's of coll meets the predicate\n";
    }
}
