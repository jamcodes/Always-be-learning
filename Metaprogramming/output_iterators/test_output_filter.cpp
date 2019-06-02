#include <iostream>
#include <string>
#include <vector>
#include "output_filter_iterator.hpp"

int main()
{
    std::vector<int> coll{1, 11, 2, 12, 22, 3, 13, 23, 33, 4, 14, 24, 34, 44};
    std::cerr << "coll: ";
    for (auto i : coll) {
        std::cerr << i << ", ";
    }
    std::vector<int> large_ints;
    auto filter = make_output_filter([](auto val) { return 10 < val; });
    std::copy(coll.cbegin(), coll.cend(), std::move(filter)(std::back_inserter(large_ints)));
    std::cerr << "\nlarge_ints: ";
    for (auto i : large_ints) {
        std::cerr << i << ", ";
    }
}
