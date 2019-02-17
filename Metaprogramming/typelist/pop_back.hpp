#pragma once

#include "typelist.hpp"
#include "reverse.hpp"


template<typename List> struct pop_back_impl;

template<template<typename...>class List, typename T, typename... Ts>
struct pop_back_impl<List<T,Ts...>>
{
    using type = push_front_t<typename pop_back_impl<List<Ts...>>::type, T>;
};

template<template<typename...>class List, typename T>
struct pop_back_impl<List<T>>
{
    using type = List<>;
};

template<template<typename...>class List>
struct pop_back_impl<List<>>
{
    using type = List<>;
};

template<typename List>
struct pop_back : pop_back_impl<List> { };

template<typename List>
using pop_back_t = typename pop_back_impl<List>::type;

namespace pop_back_unit_test
{

using lst0 = typelist<>;
using lst1 = typelist<int>;
using lst2 = typelist<int, double>;
using lst3 = typelist<int, double, char>;
static_assert(std::is_same_v<pop_back_t<lst0>,lst0>);
static_assert(std::is_same_v<pop_back_t<lst1>,lst0>);
static_assert(std::is_same_v<pop_back_t<lst2>,lst1>);
static_assert(std::is_same_v<pop_back_t<lst3>,lst2>);

} // namespace pop_back_unit_test


namespace pop_back_alt_impl
{

template<typename List>
struct pop_back
{
    using type = reverse_t<pop_front_t<reverse_t<List>>>;
};

template<typename List>
using pop_back_t = typename pop_back<List>::type;


namespace unit_test_pop_back
{

using lst0 = typelist<>;
using lst1 = typelist<int>;
using lst2 = typelist<int, double>;
using lst3 = typelist<int, double, char>;
static_assert(std::is_same_v<pop_back_t<lst0>,lst0>);
static_assert(std::is_same_v<pop_back_t<lst1>,lst0>);
static_assert(std::is_same_v<pop_back_t<lst2>,lst1>);
static_assert(std::is_same_v<pop_back_t<lst3>,lst2>);

} // unit_test_pop_back

} // pop_back_alt_impl
