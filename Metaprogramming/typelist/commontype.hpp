#pragma once

#include <utility>
#include <type_traits>

template<typename T, typename U>
struct common_type
{
    using type = decltype(true ? std::declval<T>() : std::declval<U>());
};

template<typename T, typename U>
using common_type_t = typename common_type<T,U>::type;
