#include <iostream>
#include <string>
#include <string_view>
#include <cassert>
#include "tuple_find_if.hpp"

int main()
{
    auto one{std::make_tuple(std::string{"Hello"})};
    auto two{std::make_tuple("Tuple", 2.71828182846)};
    auto const ctup{std::make_tuple(std::string{"Algorithms"}, 42, 3.14159265359)};

    struct pred {
        constexpr bool operator()(int val) const noexcept { return val == 42; }
        constexpr bool operator()(std::string_view val) const noexcept
        {
            return val.find("Hello") != std::string::npos;
        }
        constexpr bool operator()(double val) const noexcept { return val == 2.71828182846; }
    };

    auto index = tuple_find_if(one, pred{});
    auto index2 = alt::tuple_find_if(one, pred{});
    assert(index == index2);
    if (index != std::tuple_size<decltype(one)>::value) {
        std::cerr << "Tuple 'one': Found a match at index = " << index << "\n";
    }
    else {
        std::cerr << "Tuple 'one': did not find a match\n";
    }

    index = tuple_find_if(two, pred{});
    index2 = alt::tuple_find_if(two, pred{});
    assert(index == index2);
    if (index != std::tuple_size<decltype(two)>::value) {
        std::cerr << "Tuple 'two': Found a match at index = " << index << "\n";
    }
    else {
        std::cerr << "Tuple 'two': did not find a match\n";
    }

    index = tuple_find_if(ctup, pred{});
    index2 = alt::tuple_find_if(ctup, pred{});
    assert(index == index2);
    if (index != std::tuple_size<decltype(ctup)>::value) {
        std::cerr << "Tuple 'ctup': Found a match at index = " << index << "\n";
    }
    else {
        std::cerr << "Tuple 'ctup': did not find a match\n";
    }
}
