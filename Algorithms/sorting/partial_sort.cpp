#include <iostream>
#include <array>
#include <vector>
#include <iterator>
#include "auxiliary.h"


template< class RandomIt >
constexpr void partial_sort( RandomIt first, RandomIt middle, RandomIt last );

template< class RandomIt, class Compare >
constexpr void partial_sort( RandomIt first, RandomIt middle, RandomIt last,
                             Compare comp );

template< class InputIt, class RandomIt >
constexpr RandomIt partial_sort_copy( InputIt first, InputIt last,
                                      RandomIt d_first, RandomIt d_last );

template< class InputIt, class RandomIt, class Compare >
constexpr RandomIt partial_sort_copy( InputIt first, InputIt last,
                                      RandomIt d_first, RandomIt d_last,
                                      Compare comp );

/*
std::partial_sort
Rearranges elements such that the range [first, middle) contains the sorted middle -
first smallest elements in the range [first, last)
The order of equal elements is not guaranteed to be preserved. The order of the remaining
elements in the range [middle, last) is unspecified.

std::partial_sort_copy
Sorts some of the elements in the range [first, last) in ascending order,
storing the resultin the range [d_first, d_last).
At most d_last - d_first of the elements are placed sorted to the range [d_first, d_first + n).
n is the number of elements to sort (n = min(last - first, d_last - d_first)).
The order of equal elements is not guaranteed to be preserved. 
*/

class gen
{
public:
    constexpr explicit gen(int beg) noexcept
        : i_{beg} { }
    int operator()() noexcept { return i_++; }
private:
    int i_{};
};

int main()
{
    std::vector<int> coll;
    std::generate_n(std::back_inserter(coll), 4, gen{3});
    std::generate_n(std::back_inserter(coll), 4, gen{2});
    std::generate_n(std::back_inserter(coll), 4, gen{1});
    print_elements(coll);

    // sort until first five elements are sorted
    std::partial_sort(coll.begin(),                     // beginning of the range
                      std::next(coll.begin(), 5),       // end of range we want to be sorted
                      coll.end());                      // end of full range being sorted
    print_elements(coll, "coll, first five sorted: ");

    std::partial_sort(coll.begin(),
                      std::next(coll.begin(), 5),
                      coll.end(),
                      [](int x, int y){ return y < x; });   // predicate
    print_elements(coll, "coll, first five sorted with '>': ");


    std::cout << "\npartial_sort_copy:\n";
    
}
