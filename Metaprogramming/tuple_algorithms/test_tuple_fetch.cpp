#include <iostream>
#include <string>
#include "tuple_for_each.hpp"
#include "tuple_find_if.hpp"
#include "tuple_fetch.hpp"


int main()
{
    auto tup{std::make_tuple(std::string{"Algorithms"}, 42, 3.14159265359)};
    std::cerr << "tup: {";
    tuple_for_each(tup, [](auto const& el) { std::cerr << el << ", "; });
    std::cerr << "}\n";

    struct pred {
        constexpr bool operator()(int val) const noexcept { return val == 42; }
        constexpr bool operator()(std::string_view val) const noexcept
        {
            return val.find("Hello") != std::string::npos;
        }
        constexpr bool operator()(double val) const noexcept { return val == 2.71828182846; }
    };

    auto index = tuple_find_if(tup, pred{});
    auto& elem_ref = tuple_fetch(tup, index);
    
}
