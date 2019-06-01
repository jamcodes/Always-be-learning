#include <iostream>
#include <string>
#include "tuple_apply_at.hpp"
#include "tuple_find_if.hpp"
#include "tuple_for_each.hpp"

int main()
{
    auto tup{std::make_tuple(std::string{"Algorithms"}, 42, 3.14159265359)};
    std::cerr << "tup = ";
    auto const print = [](auto const& val) { std::cerr << val << ", "; };
    tuple_for_each(tup, print);

    struct pred {
        constexpr bool operator()(int val) const noexcept { return val == 42; }
        constexpr bool operator()(std::string_view val) const noexcept
        {
            return val.find("Hello") != std::string::npos;
        }
        constexpr bool operator()(double val) const noexcept { return val == 2.71828182846; }
    };

    auto index = tuple_find_if(tup, pred{});
    if (index != std::tuple_size<decltype(tup)>::value) {
        std::cerr << "Tuple 'one': Found a match at index = " << index << "\n";
    }
    else {
        std::cerr << "Tuple 'one': did not find a match\n";
    }

    auto const twice = [](int& val) noexcept { val *= 2; };
    tuple_apply_at(tup, twice, index);
    std::cerr << "after applying `twice` at index = " << index << ", tup = ";
    tuple_for_each(tup, print);
    alt::tuple_apply_at(tup, twice, index);
    std::cerr << "after applying `twice` again at index = " << index << ", tup = ";
    tuple_for_each(tup, print);

    std::cerr << "\n\ncref tuple:\n";
    auto const ctup{tup};
    auto const print_twice = [](int val) noexcept { std::cerr << val << ", " << val; };
    tuple_apply_at(ctup, print_twice, index);

    std::string dst;
    auto const get_by_move = [&dst=dst](std::string&& val) noexcept { dst = std::move(val); };
    std::cerr << "\n\nrvalue tuple:\n";
    tuple_apply_at(std::make_tuple(42, std::string{"Hello tuples"}), get_by_move, index);
    std::cerr << "dst = " << dst << "\n";
}
