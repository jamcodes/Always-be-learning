#pragma once

#include <utility>

template<template<typename...> class Expression, typename Void, typename... Ts>
struct is_detected_impl : std::false_type { };
template<template<typename...> class Expression, typename... Ts>
struct is_detected_impl<Expression, std::void_t<Expression<Ts...>>, Ts...> : std::true_type { };

template<template<typename...> class Expression, typename... Ts>
struct is_detected : is_detected_impl<Expression, void, Ts...> { };
template<template<typename...> class Expression, typename... Ts>
constexpr inline bool is_detected_v{is_detected<Expression, Ts...>::value};
