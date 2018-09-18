#include <iostream>
#include <array>
#include <algorithm>
#include "auxiliary.h"



int main()
{
    std::array<int, 14> coll{1, 2, 42, 42, 5, 11, 34, 54, 64, 42, 42, 42, 3, 9};
    print_elements(coll, "array: ");

    // find three consecutive elements with value 42
    const auto it = std::search_n(coll.cbegin(), coll.cend(),   // range
                                  3,                            // count
                                  42);                          // value
    std::cout << "search_n: first thee '42's start at index: "
        << std::distance(coll.cbegin(), it) << "\n";

    // find four consecutive even elements:
    const auto it2 = std::search_n(coll.cbegin(), coll.cend(),      // range
                                  4,                                // count
                                  0,                                // value (here an unused dummy)
                                  [](int elem, int /* value */){    // predicate (value passed here)
                                      return elem % 2 == 0;
                                    }
                                  );
    std::cout << "search_n: first four consecutive even elements start at index: "
        << std::distance(coll.cbegin(), it2) << "\n";
}
