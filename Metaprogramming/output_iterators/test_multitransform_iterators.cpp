#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "output_multitransform_iterator.hpp"

int main()
{
    std::vector<int> coll{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::cerr << "coll: ";
    for (auto i : coll) {
        std::cerr << i << ", ";
    }
    auto square_cube = make_multitransform([](auto val) noexcept { return val * val; },
                                                 [](auto val) noexcept { return val * val * val; });
    std::vector<int> squares;
    std::vector<int> cubes;
    std::copy(coll.cbegin(), coll.cend(),
              square_cube(std::back_inserter(squares), std::back_inserter(cubes)));
    std::cerr << "\nsquaes: ";
    for(auto i : squares) {
        std::cerr << i << ", ";
    }
    std::cerr << "\ncubes: ";
    for(auto i : cubes) {
        std::cerr << i << ", ";
    }
    // prints:
    // coll: 1, 2, 3, 4, 5, 6, 7, 8, 9,
    // squaes: 1, 4, 9, 16, 25, 36, 49, 64, 81,
    // cubes: 1, 8, 27, 64, 125, 216, 343, 512, 729,

}
