#include <iostream>
#include <algorithm>
#include <array>
#include "auxiliary.h"


template< class InputIt, class OutputIt >
constexpr OutputIt copy( InputIt first, InputIt last, OutputIt d_first );

template< class InputIt, class OutputIt, class UnaryPredicate >
constexpr OutputIt copy_if( InputIt first, InputIt last,
                            OutputIt d_first,
                            UnaryPredicate pred );

/*
Copies all elements in the range [first, last) starting from first and proceeding to last - 1.
The behavior is undefined if d_first is within the range [first, last).
In this case, std::copy_backward may be used instead.
*/

template< class BidirIt1, class BidirIt2 >
constexpr BidirIt2 copy_backward( BidirIt1 first, BidirIt1 last, BidirIt2 d_last );
/*
Copies the elements from the range, defined by [first, last), to another range ending at d_last.
The elements are copied in reverse order (the last element is copied first),
but their relative order is preserved.
*/

template< class InputIt, class Size, class OutputIt >
constexpr OutputIt copy_n( InputIt first, Size count, OutputIt result );
/*
Copies exactly count values from the range beginning at first to the range beginning at result.
Formally, for each non-negative integer i < n, performs *(result + i) = *(first + i).
Overlap of ranges is not permitted.
*/

int main()
{
    std::array<int, 15> coll1{};
    const auto gen = [i=0u]()mutable noexcept{ return i++; };
    std::generate_n(coll1.begin(), 8, gen);
    print_elements(coll1, "coll1: ");
    std::copy_n(std::next(coll1.cbegin()), 7, std::next(coll1.begin(), 8));
    print_elements(coll1, "coll1 after copy_n: ");
    // copy over partially overlaping ranges - use copy_backward
    std::copy_backward(std::next(coll1.cbegin(), 2), std::next(coll1.cbegin(),11), coll1.end());
    print_elements(coll1, "coll1 after copy_backward: ");

    std::vector<int> coll2;
    std::copy(coll1.cbegin(), coll1.cend(), std::back_inserter(coll2));
    print_elements(coll2, "coll2 after copy: ");
    
}
