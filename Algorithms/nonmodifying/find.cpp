#include <algorithm>
#include <array>
#include <iterator>
#include <iostream>
#include "auxiliary.h"



int main()
{
    std::array<int, 11> coll;
    std::generate(begin(coll), end(coll), rand_gen<int>{-42, 42});
    coll.front() = 42;
    coll[5] = 11;
    coll.back() = 9;
    print_elements(coll, "array: ");

    const auto it1 = std::find(coll.cbegin(), coll.cend(), 11);
    std::cout << "first element with value == 11, at offset: "
        << std::distance(coll.cbegin(), it1) << ", value == " << *it1 << "\n";
    const auto it2 = std::find_if(cbegin(coll), cend(coll), [](int i){ return i%3 == 0;});
    std::cout << "first element divisible by 3, at offset: "
        << std::distance(coll.cbegin(), it2) << ", value == " << *it2 << "\n";
    const auto it3 = std::find_if_not(coll.cbegin(), coll.cend(), [](int i){ return i%2 == 0;});
    std::cout << "first odd element at offset: " << std::distance(coll.cbegin(), it3)
        << ", value == " << *it3 << "\n";
}
