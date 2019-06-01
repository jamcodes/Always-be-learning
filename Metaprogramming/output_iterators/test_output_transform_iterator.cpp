#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "output_transform_iterator.hpp"

int main()
{
    std::vector<std::string> coll{"one", "two", "three", "four", "five"};
    std::cerr << "coll: ";
    for (auto const& s : coll) { std::cerr << s << ", "; }
    std::cerr << "\n";
    auto const twice_transformer = make_output_transformer([](std::string s) {
        s += s;
        return s;
    });

    std::vector<std::string> results{};
    std::copy(coll.cbegin(), coll.cend(), twice_transformer(std::back_inserter(results)));
    std::cerr << "results: ";
    for (auto const& s : results) { std::cerr << s << ", "; };
    std::cerr << "\n";
}
