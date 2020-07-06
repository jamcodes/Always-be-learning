#include <iostream>
#include <tuple>

#include "tuple_accumulate.hpp"

int main()
{
    std::tuple<int, int, int> vals{1, 2, 3};
    auto adder = [](int init, int val) noexcept { return init + val; };
    auto const sum = tuple_accumulate(vals, 0, adder);
    std::cout << "sum = " << sum << "\n";
    auto const altsum = tupleAccumulate(vals, 0, adder);
    std::cout << "altsum = " << sum << "\n";
}
