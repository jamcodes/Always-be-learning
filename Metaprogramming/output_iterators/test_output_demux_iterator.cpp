#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include "output_demux_iterator.hpp"

int main()
{
    std::vector<int> coll{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> mult_of_3;
    std::vector<int> mult_of_2;
    std::vector<int> mult_of_1;
    std::copy(coll.cbegin(), coll.cend(),
              demux(demux_if([](auto val) noexcept { return val % 3 == 0; })
                        .send_to(std::back_inserter(mult_of_3)),
                    demux_if([](auto val) noexcept { return val % 2 == 0; })
                        .send_to(std::back_inserter(mult_of_2)),
                    demux_if([](auto val) noexcept { return val % 1 == 0; })
                        .send_to(std::back_inserter(mult_of_1))));
    auto print = [](auto const& collection) {
        for (auto const& el : collection) {
            std::cerr << el << ", ";
        };
    };
    std::cerr << "collection: ";
    print(coll);
    std::cerr << "\nmultiples of 3: ";
    print(mult_of_3);
    std::cerr << "\nmultiples of 2: ";
    print(mult_of_2);
    std::cerr << "\nmultiples of 1: ";
    print(mult_of_1);
    // prints:
    // collection: 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    // multiples of 3: 3, 6, 9,
    // multiples of 2: 2, 4, 8, 10,
    // multiples of 1: 1, 5, 7,

}
