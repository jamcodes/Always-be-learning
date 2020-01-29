#pragma once

#include <type_traits>
#include <utility>

// This is an example of designing and implementing a strong_typedef library facility
// to allow easily defining strong typedefs that support operators interface with minimal
// bolerplate code.

template<typename Tag, typename T>
class strong_typedef
{
private:
    T value_;

public:
    explicit constexpr strong_typedef(T const& value)
        noexcept(std::is_nothrow_copy_constructible<T>::value)
        : value_{value}
    {
    }

    explicit constexpr strong_typedef(T&& value)
        noexcept(std::is_nothrow_move_constructible<T>::value)
        : value_{std::move(value)}  // std::move constexpr since C++14
    {
    }

    strong_typedef(strong_typedef const&) noexcept(std::is_nothrow_copy_constructible_v<T>) = default;
    strong_typedef(strong_typedef&&) noexcept(std::is_nothrow_move_constructible_v<T>) = default;
    strong_typedef& operator=(strong_typedef const&) noexcept(std::is_nothrow_copy_assignable_v<T>) = default;
    strong_typedef& operator=(strong_typedef&&) noexcept(std::is_nothrow_move_assignable_v<T>) = default;
    ~strong_typedef() noexcept = default;

    explicit constexpr operator T&() & noexcept { return value_; }
    explicit constexpr operator const T&() const& noexcept { return value_; }
    explicit constexpr operator T&&() && noexcept { return std::move(value_); }
    explicit constexpr operator const T&&() const&& noexcept { return std::move(value_); }

    friend constexpr void swap(strong_typedef& lhs, strong_typedef& rhs) noexcept
    {
        using std::swap;
        swap(lhs.value_, rhs.value_);
    }
};

// Utility for retrieving the underlying type of a strong_typedef, allowing for conversions
// up the inheritance hierarchy (strongly-typedef'ed types inherit from strong_typedef)
template<typename Tag, typename T>
T underlying_type_impl(strong_typedef<Tag, T>) noexcept; // no definition needed - used in
                                                         // unevaluated contexts
template<typename StrongTypedefT>
using underlying_type = decltype(
        underlying_type_impl(std::declval<std::decay_t<StrongTypedefT>>())
    );

template<typename T, typename = std::void_t<>>
struct IsStrongTypedefT : std::false_type { };

template<typename T>
struct IsStrongTypedefT<T, std::void_t<decltype(underlying_type_impl(std::declval<T>()))>>
: std::true_type
{
};

template<typename T>
constexpr inline bool IsStrongTypedef{IsStrongTypedefT<T>::value};

template <typename From, typename To>
using enable_if_convertible = typename std::enable_if<
    !std::is_same<typename std::decay<From>::type, To>::value
    && !std::is_base_of<To, typename std::decay<From>::type>::value
    && std::is_convertible<typename std::decay<From>::type, To>::value>::type;

template <typename From, typename To>
using enable_if_convertible_same = typename std::enable_if<
    std::is_convertible<typename std::decay<From>::type, To>::value>::type;

template<typename Tag, typename T>
[[nodiscard]] constexpr T& as_underlying(strong_typedef<Tag, T>& value) noexcept
{
    return static_cast<T&>(value);
}

template<typename Tag, typename T>
[[nodiscard]] constexpr T const& as_underlying(strong_typedef<Tag, T> const& value) noexcept
{
    return static_cast<T const&>(value);
}

template<typename Tag, typename T>
[[nodiscard]] constexpr T&& as_underlying(strong_typedef<Tag, T>&& value) noexcept
{
    return static_cast<T&&>(value);
}

template<typename Tag, typename T>
[[nodiscard]] constexpr T const&& as_underlying(strong_typedef<Tag, T> const&& value) noexcept
{
    return static_cast<T const&&>(value);
}

template<typename T_, bool = IsStrongTypedef<T_>>
struct underlying_or_fwd_impl {
    template<typename T>
    [[nodiscard]] constexpr inline decltype(auto) operator()(T&& t) noexcept
    {
        return as_underlying(std::forward<T>(t));
    }
};

template<typename T_>
struct underlying_or_fwd_impl<T_, false> {
    template<typename T>
    [[nodiscard]] constexpr inline decltype(auto) operator()(T&& t) noexcept
    {
        return std::forward<T>(t);
    }
};

template<typename T>
constexpr inline auto underlying_or_fwd{underlying_or_fwd_impl<T>{}};

// Implement operators, scoped and found via ADL. Each operator is composible via inheritance.
template<typename StrongTypedefT>
struct addition
{
    friend constexpr StrongTypedefT operator+(StrongTypedefT const& lhs, StrongTypedefT const& rhs) noexcept
    {
        return StrongTypedefT{as_underlying(lhs) + as_underlying(rhs)};
    }

    friend constexpr StrongTypedefT operator+(StrongTypedefT const& lhs, StrongTypedefT&& rhs) noexcept
    {
        return StrongTypedefT{
            as_underlying(lhs) + as_underlying(static_cast<StrongTypedefT&&>(rhs))
        };
    }

    friend constexpr StrongTypedefT operator+(StrongTypedefT&& lhs, StrongTypedefT const& rhs) noexcept
    {
        return StrongTypedefT{
            as_underlying(static_cast<StrongTypedefT&&>(lhs)) + as_underlying(rhs)
        };
    }

    friend constexpr StrongTypedefT operator+(StrongTypedefT&& lhs, StrongTypedefT&& rhs) noexcept
    {
        return StrongTypedefT{
            as_underlying(static_cast<StrongTypedefT&&>(lhs))
            + as_underlying(static_cast<StrongTypedefT&&>(rhs))
        };
    }

    // Would this be equivalent to the above overloads?
    // template<typename StrongLhsT, typename StrongRhsT
    //          typename = std::enable_if_t<
    //              IsStrongTypedef<StrongLhsT> && IsStrongTypedef<StrongRhsT>>>
    // friend auto operator+(StrongT&& lhs, StrongT&& rhs) noexcept
    // {
    //     using StrongTypedef = std::decay_t<StrongT>;
    //     return StrongTypedef{
    //         as_underlying(std::forward<StrongLhsT>(lhs)) + as_underlying(std::forward<StrongRhsT>(rhs))
    //     };
    // }

    // Compount operators
    friend constexpr StrongTypedefT& operator+=(StrongTypedefT& lhs, StrongTypedefT const& rhs) noexcept
    {
        as_underlying(lhs) += as_underlying(rhs);
        return lhs;
    }

    friend constexpr StrongTypedefT& operator+=(StrongTypedefT& lhs, StrongTypedefT&& rhs) noexcept
    {
        as_underlying(lhs) += as_underlying(static_cast<StrongTypedefT&&>(rhs));
        return lhs;
    }

    friend constexpr StrongTypedefT& operator+=(StrongTypedefT& lhs, StrongTypedefT&& rhs) noexcept
    {
        as_underlying(lhs) += as_underlying(static_cast<StrongTypedefT&&>(rhs));
        return lhs;
    }

    friend constexpr StrongTypedefT&& operator+=(StrongTypedefT&& lhs, StrongTypedefT const& rhs) noexcept
    {
        as_underlying(static_cast<StrongTypedefT&&>(lhs)) += as_underlying(rhs);
        return static_cast<StrongTypedefT&&>(lhs);
    }

    friend constexpr StrongTypedefT&& operator+=(StrongTypedefT&& lhs, StrongTypedefT&& rhs) noexcept
    {
        as_underlying(static_cast<StrongTypedefT&&>(lhs))
            += as_underlying(static_cast<StrongTypedefT&&>(rhs));
        return static_cast<StrongTypedefT&&>(lhs);
    }

    // Would this be equivalent to the above overloads?
    // template<typename StrongLhsT, typename StrongRhsT,
    //          typename = std::enable_if_t<
    //              IsStrongTypedef<StrongLhsT> && IsStrongTypedef<StrongRhsT>>>
    // friend decltype(auto) operator+=(StrongLhsT&& lhs, StrongRhsT&& rhs) noexcept
    // {
    //     as_underlying(std::forward<StrongLhsT>(lhs))
    //         += as_underlying(std::forward<StrongRhsT>(rhs));
    //     return std::forward<StrongLhsT>(lhs);
    // }
};

// We may want to provide support for mixed operations - allowing for some conversions

template<template<typename, typename> class StrongTypedefT>
struct mixed_addition {
    template<typename Tag, typename T, typename Other,
             typename = enable_if_convertible_same<Other&&, T>>
    friend constexpr auto operator+(StrongTypedefT<Tag, T> const& lhs, Other&& rhs) noexcept
    {
        return StrongTypedefT<Tag, T>{
            as_underlying(lhs) + underlying_or_fwd<Other>(std::forward<Other>(rhs))
        };
    }

    template<typename Tag, typename T, typename Other,
             typename = enable_if_convertible_same<Other&&, T>>
    friend constexpr auto operator+(StrongTypedefT<Tag, T>&& lhs, Other&& rhs) noexcept
    {
        return StrongTypedefT<Tag, T>{
            as_underlying(static_cast<StrongTypedefT<Tag,T>&&>(lhs))
                + underlying_or_fwd<Other>(std::forward<Other>(rhs))
        };
    }

    template<typename Tag, typename T, typename Other,
             typename = enable_if_convertible_same<Other&&, T>>
    friend constexpr auto operator+(Other&& lhs, StrongTypedefT<Tag, T> const& rhs) noexcept
    {
        return StrongTypedefT<Tag, T>{
            underlying_or_fwd<Other>(std::forward<Other>(lhs)) + as_underlying(rhs)
        };
    }

    template<typename Tag, typename T, typename Other,
             typename = enable_if_convertible_same<Other&&, T>>
    friend constexpr auto operator+(Other&& lhs, StrongTypedefT<Tag, T>&& rhs) noexcept
    {
        return StrongTypedefT<Tag, T>{
            underlying_or_fwd<Other>(std::forward<Other>(lhs))
                + as_underlying(static_cast<StrongTypedefT<Tag, T>>(rhs))
        };
    }

    template<typename Tag, typename T, typename Other,
             typename = enable_if_convertible_same<Other&&, T>>
    friend constexpr StrongTypedefT<Tag, T>& operator+=(StrongTypedefT<Tag, T>& lhs, Other&& rhs) noexcept
    {
        return as_underlying(lhs) += underlying_or_fwd<Other>(std::forward<Other>(rhs));
    }

    template<typename Tag, typename T, typename Other,
             typename = enable_if_convertible_same<Other&&, T>>
    friend constexpr StrongTypedefT<Tag, T>& operator+=(StrongTypedefT<Tag, T>&& lhs, Other&& rhs) noexcept
    {
        return as_underlying(static_cast<StrongTypedefT<Tag, T>>(lhs))
            += underlying_or_fwd<Other>(std::forward<Other>(rhs));
    }
};

// etc. for all operations we wish to support for the given strongly-typedefed type.
