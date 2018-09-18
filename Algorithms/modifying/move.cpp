#include <iostream>
#include <algorithm>
#include <array>
#include <iterator>
#include "auxiliary.h"


template< class InputIt, class OutputIt >
constexpr OutputIt move( InputIt first, InputIt last, OutputIt d_first );
/*
Moves the elements in the range [first, last), to another range beginning at d_first,
starting from first and proceeding to last - 1.
After this operation the elements in the moved-from range will still contain valid values
of the appropriate type, but not necessarily the same values as before the move.
*/

template< class BidirIt1, class BidirIt2 >
constexpr BidirIt2 move_backward( BidirIt1 first, BidirIt1 last, BidirIt2 d_last );
/*
Moves the elements from the range [first, last), to another range ending at d_last.
The elements are moved in reverse order (the last element is moved first),
but their relative order is preserved.
*/

int main()
{
    std::array<int, 9> coll1{1,2,3,4,5,6,7,8,9};
    std::vector<int> coll2;
    std::move(coll1.begin(), coll1.end(), std::back_inserter(coll2));
    print_elements(coll1, "coll1: ");
    print_elements(coll2, "coll2 after move: ");
}
