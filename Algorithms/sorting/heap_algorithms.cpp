#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <iterator>
#include "auxiliary.h"


template< class RandomIt >
constexpr void make_heap( RandomIt first, RandomIt last );

template< class RandomIt, class Compare >
constexpr void make_heap( RandomIt first, RandomIt last,
                          Compare comp );

/*
Constructs a max heap in the range [first, last). The first version of the function uses
operator< to compare the elements, the second uses the given comparison function comp.
*/

template< class RandomIt >
constexpr void push_heap( RandomIt first, RandomIt last );

template< class RandomIt, class Compare >
constexpr void push_heap( RandomIt first, RandomIt last,
                          Compare comp );

/*
Inserts the element at the position last-1 into the max heap defined by the range [first, last-1).
The first version of the function uses operator< to compare the elements,
the second uses the given comparison function comp.
The caller has to ensure that, on entry, the elements in the range [beg,end-1) are a heap
and that the new element immediately follows these elements.
*/

template< class RandomIt >
constexpr void pop_heap( RandomIt first, RandomIt last );

template< class RandomIt, class Compare >
constexpr void pop_heap( RandomIt first, RandomIt last, Compare comp );

/*
Both forms move the highest element of the heap [first,last), which is the first element, to the last
position and create a new heap from the remaining elements in the range [first,last-1).
The first version of the function uses operator< to compare the elements,
the second uses the given comparison function comp.
The caller has to ensure that, on entry, the elements in the range [beg,end) are a heap.
*/

template< class RandomIt >
constexpr void sort_heap( RandomIt first, RandomIt last );

template< class RandomIt, class Compare >
constexpr void sort_heap( RandomIt first, RandomIt last, Compare comp );

/*
Converts the max heap [first, last) into a sorted range in ascending order.
The resulting range no longer has the heap property. 
The caller has to ensure that, on entry, the elements in the range [beg,end) are a heap.
*/


int main()
{
    std::vector<int> coll;
    std::generate_n(std::back_inserter(coll), 4, [i=3]()mutable{return i++;});
    std::generate_n(std::back_inserter(coll), 4, [i=5]()mutable{return i++;});
    std::generate_n(std::back_inserter(coll), 4, [i=1]()mutable{return i++;});
    print_elements(coll, "coll: ");

    // convert collection into a heap
    std::make_heap(coll.begin(), coll.end());
    print_elements(coll, "after make_heap(): ");

    // pop next element out of the heap
    std::pop_heap(coll.begin(), coll.end());
    coll.pop_back();
    print_elements(coll, "after pop_heap(): ");

    // push new element into the heap
    coll.push_back(17);
    std::push_heap(coll.begin(), coll.end());
    print_elements(coll, "after push_heap(): ");

    // convert heap into a sorted collection (no longer a heap afterwards)
    std::sort_heap(coll.begin(), coll.end());

    print_elements(coll, "after sort_heap(): ");
}
