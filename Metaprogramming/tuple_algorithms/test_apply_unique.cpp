#include <iostream>
#include <string>
#include <tuple>
#include "tuple_apply_unique.hpp"
#include "tuple_for_each.hpp"

int main()
{
    auto tup{std::make_tuple(std::string{"Algorithms"}, 42, 3.14159265359)};
    std::cerr << "tup = ";
    auto const print = [](auto const& val) { std::cerr << val << ", "; };
    tuple_for_each(tup, print);

    struct pred {
        constexpr bool operator()(int val) const noexcept { return val % 2 == 0; }
        constexpr bool operator()(std::string_view val) const noexcept
        {
            return val.find("Hello") != std::string::npos;
        }
        constexpr bool operator()(double val) const noexcept { return val == 2.71828182846; }
    };

    auto const twice = [](int& val) noexcept { val *= 2; };
    tuple_apply_unique(tup, twice, pred{});
    std::cerr << "\nafter applying `twice`, tup = ";
    tuple_for_each(tup, print);
    alt::tuple_apply_unique(tup, twice, pred{});
    std::cerr << "\nafter applying `twice` again, tup = ";
    tuple_for_each(tup, print);

    std::cerr << "\n\ncref tuple:\n";
    auto const ctup{tup};
    auto const print_twice = [](int val) noexcept { std::cerr << val << ", " << val; };
    tuple_apply_unique(ctup, print_twice, pred{});

    int dst;
    auto const assign = [&dst = dst](int val) noexcept { dst = val; };
    std::cerr << "\n\nrvalue tuple:\n";
    tuple_apply_unique(std::make_tuple(42, std::string{"Hello tuples"}), assign, pred{});
    std::cerr << "dst = " << dst << "\n";
}
