#include "chat_message.hpp"

#include <array>
#include <iostream>
#include <charconv>
#include <utility>


int main()
{
    std::array<char, 4> data{'0', '0', '4', '2'};
    std::size_t val{0};
    std::from_chars_result fr{std::from_chars(static_cast<char*>(data.data()),
                                              static_cast<char*>(data.data() + data.size()), val)};
    if (fr.ec != std::errc{}) {
        std::cout << "std::from_chars error: " << std::make_error_code(fr.ec).message() << std::endl;
        val = 0;
    }
    std::cout << "val = " << val << std::endl;
}
