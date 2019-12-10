#include <iostream>
#include <type_traits>

// When applying the detection-idiom it may be necessary to rely on ADL - we may want to check
// if some expression can be called on a given type, taking into account free functions provided
// in some namespace and also taking ADL into consideration.

// We can do so, without polluting the given namespace with using declarations.
// Given a `is_detected` idiom template:
template<typename...> using void_t = void;

template<template<typename...> class Expression, typename Void, typename... Ts>
struct is_detected_impl : std::false_type { };

template<template<typename...> class Expression, typename... Ts>
struct is_detected_impl<Expression, void_t<Expression<Ts...>>, Ts...> : std::true_type { };

template<template<typename...> class Expression, typename... Ts>
using is_detected_t = is_detected_impl<Expression, void, Ts...>;
template<template<typename...> class Expression, typename... Ts>
inline constexpr bool is_detected{is_detected_t<Expression, Ts...>::value};

// If and we want to check if calling `begin` and `end` on a given type are valid operations
// we can take ADL into consideration by doing the following:
namespace detail
{
    using std::begin;
    using std::end;

    template<typename T>
    using begin_expression = decltype(begin(std::declval<T>()));
    template<typename T>
    using end_expression = decltype(end(std::declval<T>()));
} // namespace detail

template<typename T>
inline constexpr bool IsRange{is_detected<begin_expression, T> && is_detected<end_expression, T>};

int main()
{

}
