#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <iterator>
#include "auxiliary.h"


template< class InputIt, class RandomIt >
constexpr RandomIt partial_sort_copy( InputIt first, InputIt last,
                                      RandomIt d_first, RandomIt d_last );

template< class InputIt, class RandomIt, class Compare >
constexpr RandomIt partial_sort_copy( InputIt first, InputIt last,
                                      RandomIt d_first, RandomIt d_last,
                                      Compare comp );
/*
std::partial_sort_copy
Sorts some of the elements in the range [first, last) in ascending order,
storing the resultin the range [d_first, d_last).
At most d_last - d_first of the elements are placed sorted to the range [d_first, d_first + n).
n is the number of elements to sort (n = min(last - first, d_last - d_first)).
The order of equal elements is not guaranteed to be preserved.
Returns an iterator to the element defining the upper boundary of the sorted range.
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
    std::vector<int> coll1;
    std::array<int, 6> coll6;
    std::array<int, 30> coll30{};
    std::generate_n(std::back_inserter(coll1), 4, gen{3});
    std::generate_n(std::back_inserter(coll1), 4, gen{2});
    std::generate_n(std::back_inserter(coll1), 4, gen{1});
    print_elements(coll1, "coll1: ");

    // copy elements of coll1 sorter into coll6
    const auto it6 =  std::partial_sort_copy(coll1.cbegin(), coll1.cend(),
                                             coll6.begin(), coll6.end());
    print_elements(coll6, "coll6: ");
    std::cout << "iterator returned by partial_sort_copy == to coll6.end() = "
        << std::boolalpha << (it6 == coll6.end()) << "\n";
    
    // copy elements of coll1 sorted into coll30
    const auto it30 = std::partial_sort_copy(coll1.cbegin(), coll1.cend(),
                                             coll30.begin(), coll30.end(),
                                             [](int a, int b){ return b < a; });

    std::cout << "copied sorted elements of coll30: ";
    std::copy(coll30.begin(), it30, std::ostream_iterator<int>(std::cout, ", "));
    std::cout << "\n";
    print_elements(coll30, "coll30: ");
}
