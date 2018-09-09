#pragma once

#include <type_traits>

template<typename...>
using void_t = void;

template<template<typename...> class Expression, typename /* = void */, typename... Ts>
struct is_detected_impl : std::false_type { };

template<template<typename...> class Expression, typename... Ts>
struct is_detected_impl<Expression, void_t<Expression<Ts...>>, Ts...> : std::true_type { };

template<template<typename...> class Expression, typename... Ts>
using is_detected = is_detected_impl<Expression, void, Ts...>;

template<template<typename...> class Expression, typename... Ts>
constexpr inline auto is_detected_v = is_detected<Expression, Ts...>::value;
