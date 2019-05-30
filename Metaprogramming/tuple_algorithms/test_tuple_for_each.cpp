#include <iostream>
#include <string>
#include "tuple_for_each.hpp"

int main()
{
    auto one{std::make_tuple(std::string{"Hello"})};
    auto two{std::make_tuple("Tuple", 2.71828182846)};
    auto const ctup{std::make_tuple(std::string{"Algorithms"}, 42, 3.14159265359)};
    auto const func = [](auto const& val) { std::cerr << val << ", "; };
    tuple_for_each(one, func);
    std::cerr << "\n";
    tuple_for_each(two, func);
    std::cerr << "\n";
    tuple_for_each(ctup, func);

    std::cerr << "\nalt:\n";
    alt::tuple_for_each(one, func);
    std::cerr << "\n";
    alt::tuple_for_each(two, func);
    std::cerr << "\n";
    alt::tuple_for_each(ctup, func);

    std::cerr << "\nalt2:\n";
    alt2::tuple_for_each(one, func);
    std::cerr << "\n";
    alt2::tuple_for_each(two, func);
    std::cerr << "\n";
    alt2::tuple_for_each(ctup, func);

    std::cerr << "\nalt3:\n";
    alt3::tuple_for_each(one, func);
    std::cerr << "\n";
    alt3::tuple_for_each(two, func);
    std::cerr << "\n";
    alt3::tuple_for_each(ctup, func);

    std::cerr << "\nalt4:\n";
    alt4::tuple_for_each(one, func);
    std::cerr << "\n";
    alt4::tuple_for_each(two, func);
    std::cerr << "\n";
    alt4::tuple_for_each(ctup, func);
}
