#include <iostream>
#include <array>
#include <algorithm>
#include "auxiliary.h"



int main()
{
    std::array<int, 42> coll;
    std::generate(coll.begin(), coll.end(), rand_gen<int>(-42, 42));
    print_elements(coll, "array elements: ");

    const auto num = std::count(coll.cbegin(), coll.cend(), 42);
    std::cout << "Number of elements equal to 42: " << num << "\n";

    const auto num2 = std::count_if(coll.cbegin(), coll.cend(), [](int el){return el > 9;});
    std::cout << "Number of elements greater than 9: " << num2 << "\n";

    const auto num3 = std::count_if(coll.cbegin(), coll.cend(), [](int el){ return el % 2 == 0;});
    std::cout << "Number of even elements: " << num3 << "\n";
}
