#include <iostream>
#include <string>
#include <type_traits>

template<typename... Ts> struct typelist { };

template<char const str[]>
struct str_constant {
    static constexpr inline auto value{str};
};

constexpr char const str1[] = "str1";
constexpr char const str2[] = "str2";
using strc1 = str_constant<str1>;
using strc2 = str_constant<str2>;

template<typename T>
void try_one(std::string const& str)
{
    if (str == T::value) {
        std::cerr << "hello from " << T::value << "\n";
    }
}
template<typename... Ts>
void try_all(typelist<Ts...>, std::string const& str)
{
    (... , try_one<Ts>(str));
}

constexpr char const one[]{"one"};
constexpr char const two[]{"two"};
constexpr char const three[]{"three"};

int main()
{
    using str_list = typelist<
                            str_constant<one>,
                            str_constant<two>,
                            str_constant<three>
                            >;
    try_all(str_list{}, std::string("two"));
    try_all(str_list{}, std::string("one"));
    try_all(str_list{}, std::string("three"));
}
