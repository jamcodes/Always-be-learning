#include <iostream>
#include <string>
#include <vector>
#include "output_partition_iterator.hpp"

int main()
{
    std::vector<int> coll{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::cerr << "coll: ";
    for (auto i : coll) {
        std::cerr << i << ", ";
    }
    std::vector<int> even;
    std::vector<int> odd;
    auto const evenPartition = partition([](auto val) noexcept { return val % 2 == 0; });
    std::copy(coll.cbegin(), coll.cend(),
              evenPartition(std::back_inserter(even), std::back_inserter(odd)));
    std::cerr << "\neven: ";
    for (auto i : even) { std::cerr << i << ", "; }
    std::cerr << "\nodd: ";
    for (auto i : odd) { std::cerr << i << ", "; }
    // prints:
    // coll: 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    // even: 2, 4, 6, 8, 10,
    // odd: 1, 3, 5, 7, 9,

}
