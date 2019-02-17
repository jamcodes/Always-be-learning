#pragma once
#include <type_traits>

template<typename List>
struct is_empty : std::false_type { };

template<template<typename...>class List>
struct is_empty<List<>> : std::true_type { };

template<typename List>
constexpr inline auto is_empty_v = is_empty<List>::value;
