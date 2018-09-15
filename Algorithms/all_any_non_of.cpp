#include <iostream>
#include <algorithm>
#include <array>
#include <iterator>
#include "auxiliary.h"


template< class InputIt, class UnaryPredicate >
constexpr bool all_of( InputIt first, InputIt last, UnaryPredicate p );

template< class InputIt, class UnaryPredicate >
constexpr bool any_of( InputIt first, InputIt last, UnaryPredicate p );

template< class InputIt, class UnaryPredicate >
constexpr bool none_of( InputIt first, InputIt last, UnaryPredicate p );

/* 
all_of:  Checks if unary predicate p returns true for all elements in the range [first, last).
         Returns true if range is empty
any_of:  Checks if unary predicate p returns true for at least one element in the range [first, last).
         Returns false if range is empty
none_of: Checks if unary predicate p returns true for no elements in the range [first, last).
         Returns true if range is empty
*/


int main()
{
    std::array<int, 11> coll{};
    std::generate(coll.begin(), coll.end(), rand_gen{0, 42});
    print_elements(coll, "coll: ");

    const auto isEven = [](int el) { return el % 2 == 0; };
    std::cout << std::boolalpha
        << "all_of even?: " << std::all_of(coll.cbegin(), coll.cend(), isEven) << "\n"
        << "any_of even?: " << std::any_of(coll.cbegin(), coll.cend(), isEven) << "\n"
        << "none_of even?: " << std::none_of(coll.cbegin(), coll.cend(), isEven) << "\n";
}
