#include <iostream>
#include <numeric>
#include <array>
#include "auxiliary.h"


template< class ForwardIterator, class T >
void iota( ForwardIterator first, ForwardIterator last, T value );

/*
Fills the range [first, last) with sequentially increasing values,
starting with value and repetitively evaluating ++value.
*/

int main()
{
    std::array<int, 9> coll;
    std::iota(coll.begin(), coll.end(), 42);
    print_elements(coll, "coll: ");
}
