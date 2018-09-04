#include <iostream>
#include <vector>
#include <type_traits>
#include <utility>

/**
 * Many uses of SFINAE come down to detecting if a particular member if defined for an
 * instance of a given type. It is often done like so:
 */
template<typename...>
using void_t = void;

template<typename T, typename U>
using has_foo_impl = decltype(
    std::declval<T>().foo(std::declval<U>())    // any expression
);

template<typename T, typename U, typename = void>
struct has_foo : std::false_type { };

template<typename T, typename U>
struct has_foo<T, U, void_t<has_foo_impl<T, U>>> : std::true_type { };

/**
 * Let's try to factor out the common parts of the code.
 * For a single-type expression tests we could do:
 */
template<typename T, template<typename> class Expression, typename = void>
struct is_detected_first_try : std::false_type { };

template<typename T, template<typename> class Expression>
struct is_detected_first_try<T, Expression, void_t<Expression<T>>> : std::true_type { };

// has_increment could be defined using the above template as follows:
template<typename T>
using increment_expression = decltype(++std::declval<T&>());
template<typename T>
using has_pre_increment = is_detected_first_try<T, increment_expression>;
template<typename T>
inline constexpr auto has_pre_increment_v = has_pre_increment<T>::value;

static_assert(has_pre_increment_v<int>, "has_pre_increment<int> test failed");
static_assert(has_pre_increment_v<double*>, "has_pre_increment<double*> test failed");
static_assert(has_pre_increment_v<std::vector<int>::iterator>,
    "has_pre_increment<iterator> test failed");


// Could we further generalize to avoid any type of expression?
// Yes, but the variadic parameter pack needs to be moved to the back, so it doesn't eat-up
// all of the parameters. This makes it impossible to have a default argument for SFINAE purposes
// We can move the details to an implementation template, to avoid polutting the interface:
template<template<typename...> class Expression, typename /* = void */, typename... Ts>
struct is_detected_impl : std::false_type { };

template<template<typename...> class Expression, typename... Ts>
struct is_detected_impl<Expression, void_t<Expression<Ts...>>, Ts...> : std::true_type { };

// if we stopped here we would need to be explicit about all of the template parameters
// in the specialization.
// We could define 'are_assignable'
template<typename T, typename U>
using assign_expression = decltype(std::declval<T&>() = std::declval<U&>());
template<typename T, typename U>
using are_assignable = is_detected_impl<assign_expression, void, T, U>;
// This works but we needed to be explicit about passing in void (or void_t<>) and T, U
static_assert(are_assignable<double, int>::value, "are_assignable<double, int> test failed");

// Going further we can clean this up:
template<template<typename...> class Expression, typename... Ts>
using is_detected = is_detected_impl<Expression, void, Ts...>;

template<template<typename...> class Expression, typename... Ts>
constexpr inline auto is_detected_v = is_detected<Expression, Ts...>::value;

// is_assignable then becomes (reusing the assign_expression)
template<typename T, typename U>
using is_assignable = is_detected<assign_expression, T, U>;
template<typename T, typename U>
constexpr inline auto is_assignable_v = is_assignable<T,U>::value;
static_assert(is_assignable_v<double, int>, "is_assignable<double, int> test failed");
static_assert(is_assignable_v<double*, int> == false,
    "is_assignable<double*, int> == false test failed");

// or just...
template<typename T, typename U>
constexpr inline auto is_assignable_alt_v = is_detected_v<assign_expression, T, U>;
static_assert(is_assignable_alt_v<double, int>, "is_assignable<double, int> test failed");
static_assert(is_assignable_alt_v<double*, int> == false,
    "is_assignable<double*, int> == false test failed");

int main()
{

}
