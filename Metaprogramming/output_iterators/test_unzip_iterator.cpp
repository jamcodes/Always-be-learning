#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>
#include "output_unzip_iterator.hpp"

std::ostream& operator<<(std::ostream& os, std::tuple<int, char, std::string> const& tup)
{
    return os << "{" << std::get<0>(tup) << ", " << std::get<1>(tup) << ", " << std::get<2>(tup)
              << "}";
}

int main()
{
    std::vector<std::tuple<int, char, std::string>> coll{{1, 'a', "first"},
                                                         {2, 'b', "second"},
                                                         {3, 'c', "third"},
                                                         {4, 'd', "fourth"},
                                                         {5, 'e', "fifth"}};
    std::cerr << "coll:\n";
    for (auto const& el : coll) {
        std::cerr << el << ", ";
    }
    std::vector<int> ints;
    std::vector<char> chars;
    std::vector<std::string> strings;
    std::copy(
        coll.cbegin(), coll.cend(),
        unzip(std::back_inserter(ints), std::back_inserter(chars), std::back_inserter(strings)));
    std::cerr << "\nresults:\n";
    std::cerr << "ints: ";
    for (auto i : ints) { std::cerr << i << ", "; }
    std::cerr << "\nchars: ";
    for (auto c : chars) { std::cerr << c << ", "; }
    std::cerr << "\nstrings: ";
    for (auto const& s : strings) { std::cerr << s << ", "; }

    // using alternative implementation using multitransform
    std::vector<int> ints2;
    std::vector<char> chars2;
    std::vector<std::string> strings2;
    std::copy(
        coll.crbegin(), coll.crend(),
        alt::unzip(std::back_inserter(ints2), std::back_inserter(chars2), std::back_inserter(strings2))
    );
    std::cerr << "\n\nresults using alt::unzip:\n";
    std::cerr << "ints2: ";
    for (auto i : ints) { std::cerr << i << ", "; }
    std::cerr << "\nchars2: ";
    for (auto c : chars) { std::cerr << c << ", "; }
    std::cerr << "\nstrings2: ";
    for (auto const& s : strings) { std::cerr << s << ", "; }
}
