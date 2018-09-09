#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <iterator>
#include "auxiliary.h"


int main()
{
    std::vector<int> coll;
    const std::array<int, 4> subcoll{3,4,5,6};
    const auto gen = [i=0]()mutable noexcept{return i++;};
    std::generate_n(std::back_inserter(coll), 7, gen);
    std::generate_n(std::back_inserter(coll), 8, gen);

    print_elements(coll, "vector: ");
    print_elements(subcoll, "array: ");

    // search for first occurence of subcoll in coll
    const auto it1 = std::search(coll.cbegin(), coll.cend(),        // range
                                 subcoll.cbegin(), subcoll.cend()); // subrange
    std::cout << "subcoll found starting at position: "
        << std::distance(coll.cbegin(), it1) << "\n";

    // search for an occurence of subrange that meets the given criterion
    const auto it2 = std::search(coll.cbegin(), coll.cend(),                      // range
                                 subcoll.cbegin(), subcoll.cend(),                // subrange
                                 [](int lhs, int rhs){ return lhs % rhs > 0; });  // predicate
    if (it2 != coll.cend()) {
    std::cout << "subrange meeting given predicate starts at index: "
        << std::distance(coll.cbegin(), it2) << "\n";
    } else {
        std::cout << "subrange meeting given predicate not found\n";
    }
}

