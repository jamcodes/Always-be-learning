#pragma once
#include <cstddef>
#include <type_traits>

// Strongly typed enums don't decay to integral types like c-style enums do. Any operators
// that one might want to use with a given enum need to have an overload implemented for the given
// enum type. Implementing operators over and over can be avoided using templates and SFINAE.
// Using a simple enabler-trait a generic implementation is made available after specializing
// the enabled for a given enum.

template <typename E>
struct enableEnumOperators {
    static constexpr bool value{false};
};

template <typename IntegerType, typename E>
[[nodiscard]] inline constexpr std::enable_if_t<enableEnumOperators<E>::value, IntegerType>
toInteger(const E e) noexcept
{
    static_assert(std::is_integral_v<IntegerType>, "toInteger may only cast to integral types");
    return static_cast<IntegerType>(e);
}

template <typename E>
std::enable_if_t<enableEnumOperators<E>::value, E> operator|(E const lhs, E const rhs) noexcept
{
    using Underlying = typename std::underlying_type<E>::type;
    return static_cast<E>(static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs));
}

template <typename E>
std::enable_if_t<enableEnumOperators<E>::value, E> operator&(E const lhs, E const rhs) noexcept
{
    using Underlying = typename std::underlying_type<E>::type;
    return static_cast<E>(static_cast<Underlying>(lhs) & static_cast<Underlying>(rhs));
}

template <typename E, typename IntegerType>
constexpr std::enable_if_t<enableEnumOperators<E>::value, E>& operator<<=(
    E& e, IntegerType const shift) noexcept
{
    using Underlying = typename std::underlying_type<E>::type;
    return e = E{static_cast<Underlying>(e) << shift};
}

template <typename E, typename IntegerType>
constexpr std::enable_if_t<enableEnumOperators<E>::value, E> operator<<(
    E const e, IntegerType const shift) noexcept
{
    using Underlying = typename std::underlying_type<E>::type;
    return E{static_cast<Underlying>(e) << __shift};
}
template <typename E>
constexpr std::enable_if_t<enableEnumOperators<E>::value, E>& operator|=(E& l, E const r) noexcept
{
    using Underlying = typename std::underlying_type<E>::type;
    return l = E{static_cast<Underlying>(l) | static_cast<Underlying>(r)};
}
template <typename E>
constexpr std::enable_if_t<enableEnumOperators<E>::value, E> operator|(E const l,
                                                                       E const r) noexcept
{
    using Underlying = typename std::underlying_type<E>::type;
    return E{static_cast<Underlying>(l) | static_cast<Underlying>(r)};
}

// etc. for other operators one might wish to use,
