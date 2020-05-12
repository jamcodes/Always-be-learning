#pragma once

#include <type_traits>
#include <utility>


// Empty base optimization as implemented in the standard library

struct Zero_then_variadic_args_t {
};  // tag type for value-initializing first, constructing second from remaining args

struct One_then_variadic_args_t {
};  // tag type for constructing first from one arg, constructing second from remaining args

template <class T1, class T2, bool = std::is_empty_v<T1> && !std::is_final_v<T1>>
class CompressedPair final : private T1 {
public:
    T2 myval2_;

    using Base = T1;

    template <class... Other2>
    constexpr explicit CompressedPair(Zero_then_variadic_args_t, Other2&&... val2) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T1>,
                           std::is_nothrow_constructible<T2, Other2...>>)
        : T1(), myval2_(std::forward<Other2>(val2)...)
    {
    }

    template <class Other1, class... Other2>
    constexpr CompressedPair(One_then_variadic_args_t, Other1&& val1, Other2&&... val2) noexcept(
        std::conjunction_v<std::is_nothrow_constructible<T1, Other1>,
                           std::is_nothrow_constructible<T2, Other2...>>)
        : T1(std::forward<Other1>(val1)), myval2_(std::forward<Other2>(val2)...)
    {
    }

    constexpr T1& get_first() & noexcept { return *this; }
    constexpr const T1& get_first() const& noexcept { return *this; }
    constexpr T1&& get_first() && noexcept { return *this; }
    constexpr T1 const&& get_first() const&& noexcept { return *this; }
};

// Trivial implementation, that pretty much has the same effect?
// Less safe - converts to T1
// Aggregate - no utility for controlling passing arguments for construction of T1
//             the first argument needs to initialize T1
template <class T1, class T2, bool = std::is_empty_v<T1> && !std::is_final_v<T1>>
struct EmptyBaseOptimized final : public T1 {
    T2 val_;
    using Base = T1;
};
