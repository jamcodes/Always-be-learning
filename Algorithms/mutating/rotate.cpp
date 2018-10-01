#include <iostream>
#include <array>
#include <algorithm>
#include <iterator>
#include "auxiliary.h"


template< class ForwardIt >
constexpr ForwardIt rotate( ForwardIt first, ForwardIt n_first, ForwardIt last );

template< class ForwardIt, class OutputIt >
constexpr OutputIt rotate_copy( ForwardIt first, ForwardIt n_first,
                                ForwardIt last, OutputIt d_first );

/*
std::rotate
Performs a left rotation on a range of elements.
Specifically, std::rotate swaps the elements in the range [first, last)
in such a way that the element n_first becomes the first element of the new range
and n_first - 1 becomes the last element.
A precondition of this function is that [first, n_first) and [n_first, last) are valid ranges.

std::rotate_copy
Copies the elements from the range [first, last), to another range beginning at d_first
in such a way, that the element n_first becomes the first element of the new range
and n_first - 1 becomes the last element. 
*/


int main()
{
    std::array<int, 11> coll;
    const auto gen = [i=0u]()mutable noexcept{return i++;};
    std::generate(coll.begin(), coll.end(), gen);
    print_elements(coll, "coll: ");     // coll: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,

    std::rotate(coll.begin(),               // beginning of range
                std::next(coll.begin()),    // new first element
                coll.end());                // end of range
    print_elements(coll, "one left: ");     // one left: 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0,
    
    std::rotate(coll.begin(),                   // beginning of range
                std::prev(coll.end(), 2),       // new first element
                coll.end());                    // end of range
    print_elements(coll, "two \"right\": ");    // really size()-2 left
                                                // two "right": 10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,

    std::cout << "rotated '4' to be first: ";
    std::rotate_copy(coll.begin(),
                     std::find(coll.begin(), coll.end(), 4),
                     coll.end(),
                     std::ostream_iterator<int>(std::cout, ", "));
    // rotated '4' to be first: 4, 5, 6, 7, 8, 9, 10, 0, 1, 2, 3,
}
