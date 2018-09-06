#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <iterator>
#include "auxiliary.h"





int main()
{

    std::vector<int> coll;
    coll.reserve(11);
    std::fill_n(std::back_inserter(coll), 11, rand_gen<int>{-42,42});
    print_elements(coll, "coll: ");

    const std::vector<int>::const_iterator itmin1 = std::min_element(coll.cbegin(), coll.cend());
    std::cout << "min_element: " << *itmin1 << "\n";
    const auto itmax1 = std::max_element(coll.cbegin(), coll.cend());
    std::cout << "max_element: " << *itmax1 << "\n";

    const auto abslt = [](int lhs, int rhs){ return abs(lhs) < abs(rhs); };
    const auto itmin2 = std::min_element(coll.cbegin(), coll.cend(), abslt);
    std::cout << "min_element (absolute value cmp): " << *itmin2 << "\n";
    const auto itmax2 = std::max_element(coll.cbegin(), coll.cend(), abslt);
    std::cout << "max_element (absolute value cmp): " << *itmax2 << "\n";

    using citer = std::vector<int>::const_iterator;
    const std::pair<citer,citer> minmax1 = std::minmax_element(coll.cbegin(), coll.cend());
    std::cout << "minmax_element-s: {" << *minmax1.first << "," << *minmax1.second << "}\n";
    const auto minmax2 = std::minmax_element(coll.cbegin(), coll.cend(), abslt);
    std::cout << "minmax_element-s (absolute value cmp): {"
        << *minmax2.first << "," << *minmax2.second << "}\n";
}
