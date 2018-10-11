#include <iostream>
#include <array>
#include <numeric>
#include "auxiliary.h"


template< class InputIt1, class InputIt2 >
constexpr bool includes( InputIt1 first1, InputIt1 last1,
                         InputIt2 first2, InputIt2 last2 );

template< class InputIt1, class InputIt2, class Compare >
constexpr bool includes( InputIt1 first1, InputIt1 last1,
                         InputIt2 first2, InputIt2 last2, Compare comp );

/*
std::includes
Returns true if the sorted range [first2, last2) is a subsequence of the sorted range
[first1, last1). (A subsequence need not be contiguous.)
*/


int main()
{
    std::array<int, 10> coll;
    std::array<int, 3> search = {3, 4, 7};
    std::iota(coll.begin(), coll.end(), 1);
    print_elements(coll, "coll: ");
    print_elements(search, "search: ");

    // check whether all elements in search are also in coll
    if (std::includes(coll.cbegin(), coll.cend(),
                      search.cbegin(), search.cend())) {
        std::cout << "all elements of search are also in coll\n";
    }
    else {
        std::cout << "not all elements of search are also in coll\n";
    }
}
