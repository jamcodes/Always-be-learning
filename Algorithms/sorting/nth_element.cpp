#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <iterator>
#include "auxiliary.h"


template< class RandomIt >
constexpr void nth_element( RandomIt first, RandomIt nth, RandomIt last );

template< class RandomIt, class Compare >
constexpr void nth_element( RandomIt first, RandomIt nth, RandomIt last,
                            Compare comp );

/*
std::nth_element
is a partial sorting algorithm that rearranges elements in [first, last) such that:
- The element pointed at by nth is changed to whatever element would occur in that position
  if [first, last) were sorted.
- All of the elements before this new nth element are less than or equal to the elements
  after the new nth element.
 */


struct gen
{
    constexpr explicit gen(int beg) noexcept
        : i_{beg} { }
    int operator()() noexcept { return i_++; }
    int i_{};
};


int main()
{
    std::vector<int> coll;
    std::generate_n(std::back_inserter(coll), 4, gen{3});
    std::generate_n(std::back_inserter(coll), 2, gen{4});
    std::generate_n(std::back_inserter(coll), 4, gen{1});
    print_elements(coll, "coll: ");

    // extract four lowest elements
    std::nth_element(coll.begin(),                  // beginning of range
                     std::next(coll.begin(), 3),    // element that should be sorted correctly
                     coll.end());                   // end of range

    // print them
    std::cout << "the four lowest element are: ";
    std::copy(coll.cbegin(), std::next(coll.cbegin(), 4),
              std::ostream_iterator<int>(std::cout, ", "));
    std::cout << "\n";

    // extract the four highest elements
    std::nth_element(coll.begin(),
                     std::next(coll.begin(), 3),
                     coll.end(),
                     [](int a, int b){ return b < a; });

    std::cout << "the four highest elements are: ";
    std::copy(coll.cbegin(), std::next(coll.cbegin(), 4),
              std::ostream_iterator<int>(std::cout, ", "));
    std::cout << "\n";
}
