#include <iostream>
#include "strong_typedef.hpp"

struct Meter : strong_typedef<Meter, int>
{
    // inherit constructors
    using strong_typedef::strong_typedef;
};

int main()
{
    Meter a{42};
    Meter b{11};
    static_cast<int&>(a) += static_cast<int const&>(b);
    // a += b;  // Illegal due to `explicit` conversions in strong_typedef
    std::cerr << "a += b -> " << static_cast<int const&>(a) << "\n";
}
