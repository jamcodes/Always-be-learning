#pragma once

#include "typelist.hpp"
#include "erase.hpp"

#include <type_traits>


template<typename List> struct remove_duplicates_impl;

template<template<typename...> class List, typename T, typename... Ts>
struct remove_duplicates_impl<List<T, Ts...>> {
private:
    using pruned_tail = typename remove_duplicates_impl<List<Ts...>>::type;
public:
    using type = push_front_t<erase_t<pruned_tail, T>, T>;
};

template<template<typename...> class List, typename T>
struct remove_duplicates_impl<List<T>> {
    using type = List<T>;
};

template<template<typename...> class List>
struct remove_duplicates_impl<List<>> {
    using type = List<>;
};

template<typename List>
struct remove_duplicates : remove_duplicates_impl<List>
{
};

template<typename List>
using remove_duplicates_t = typename remove_duplicates_impl<List>::type;

namespace remove_duplicates_unit_test
{

using lst1 = typelist<int>;
using lst2 = typelist<int, char>;
using lst3a = typelist<int, char, int>;
using lst3b = typelist<char, int, double>;
using lst3c = typelist<int, int, int>;

static_assert(std::is_same_v<remove_duplicates_t<lst1>, lst1>);
static_assert(std::is_same_v<remove_duplicates_t<lst2>, lst2>);
static_assert(std::is_same_v<remove_duplicates_t<lst3a>, typelist<int, char>>);
static_assert(std::is_same_v<remove_duplicates_t<lst3b>, lst3b>);
static_assert(std::is_same_v<remove_duplicates_t<lst3c>, typelist<int>>);

} // remove_duplicates_unit_test
