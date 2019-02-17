#pragma once

#include <type_traits>

template<typename T, typename U> struct is_same : std::false_type
{
};

template<typename T>
struct is_same<T, T> : std::true_type
{
};

template<typename T, typename U>
constexpr inline auto is_same_v{is_same<T, U>::value};

namespace is_same_unit_test
{

    static_assert(is_same_v<int, int>);
    static_assert(is_same_v<int, char> == false);
    static_assert(is_same_v<int, int const> == false);

} // is_same_unit_test
