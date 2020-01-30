#pragma once

#include <type_traits>
#include <utility>

// Using structured bindings to retrieve the type of the underlying variable(s) without the need
// to know the variable name.
// This could be used to implement a strong-typedef library, without relying on a specific baseclass
template <typename Tag, typename T>
struct strong_typedef {
    using value_type = T;
    T value;
};

template <typename T>
constexpr auto underlying_type_impl(T const& value) noexcept
{
    auto [v]{value};
    return v;
}
template <typename T>
using underlying_type = decltype(underlying_type_impl(std::declval<T>()));

template <typename U>
struct Eq {
    friend constexpr bool operator==(U const& l, U const& r) noexcept
    {
        auto const& [vl] = l;
        auto const& [vr] = r;
        return vl == vr;
    }
    friend constexpr bool operator!=(U const& l, U const& r) noexcept { return !(l == r); }
};

template<typename T>
struct addition {
    friend constexpr T& operator+=(T& l, T const& r) noexcept
    {
        auto& [vl]{l};
        auto const& [vr]{r};
        vl += vr;
        return l;
    }

    friend constexpr T operator+(T l, T const& r) noexcept
    {
        auto&& [vl]{;};
        auto const& [vr]{r};
        vl += vr;
        return l;
    }
};
