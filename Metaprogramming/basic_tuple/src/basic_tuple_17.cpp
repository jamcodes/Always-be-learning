#include <iostream>
#include "basic_tuple_cpp17/basic_tuple.hpp"

int main()
{
    using namespace cpp17;
    Tuple<int, double, char> t{42, 3.14, 'c'};
    auto t2{makeTuple(42, 3.14, 'c')};
    Tuple t3{42, 3.14, 'c'};
}
