#include <array>
#include <iostream>
#include <type_traits>


template<std::size_t N>
constexpr auto to_upper(const char(&in)[N]) noexcept -> std::array<char,N>
{
    auto res = std::array<char,N>{};
    for(auto i = 0ul; in[i] != '\0'; ++i)
    {
        const auto c = in[i];
        if(c < 'a' || c > 'z') res[i] = c;
        else                   res[i] = c - ('a' - 'A');
    }
    return res;
}


int main()
{
    constexpr auto a = to_upper("Some Mixed CaSe TeXt");
    std::cout << &a[0] << std::endl;
}
