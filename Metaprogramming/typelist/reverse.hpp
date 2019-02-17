#pragma once

#include "is_empty.hpp"
#include "typelist.hpp"


template<typename List> struct reverse_impl;

template<template<typename...>class List, typename T, typename... Ts>
struct reverse_impl<List<T,Ts...>>
{
    using type = push_back_t<typename reverse_impl<List<Ts...>>::type, T>;
};

template<template<typename...>class List>
struct reverse_impl<List<>>
{
    using type = List<>;
};

template<typename List> struct reverse : reverse_impl<List> { };

template<typename List>
using reverse_t = typename reverse_impl<List>::type;


namespace reverse_unit_test
{
    using lst0 = typelist<>;
    using lst1 = typelist<int>;
    using lst2 = typelist<int,char>;
    using lst3 = typelist<int,char,double>;
    static_assert(std::is_same_v<reverse_t<lst0>,lst0>);
    static_assert(std::is_same_v<reverse_t<lst1>,lst1>);
    static_assert(std::is_same_v<reverse_t<lst2>,typelist<char,int>>);
    static_assert(std::is_same_v<reverse_t<lst3>,typelist<double,char,int>>);
} // reverse_unit_test


namespace reverse_alt_impl
{

template<typename List, bool = is_empty_v<List>>
struct reverse;

template<typename List>
using reverse_t = typename reverse<List>::type;

// recursive case
template<typename List>
struct reverse<List, false> : push_back<reverse_t<pop_front_t<List>>, front_t<List>> { };

// basis case
template<typename List>
struct reverse<List, true>
{
    using type = List;
};

namespace unit_test_reverse
{
    using lst0 = typelist<>;
    using lst1 = typelist<int>;
    using lst2 = typelist<int,char>;
    using lst3 = typelist<int,char,double>;
    static_assert(std::is_same_v<reverse_t<lst0>,lst0>);
    static_assert(std::is_same_v<reverse_t<lst1>,lst1>);
    static_assert(std::is_same_v<reverse_t<lst2>,typelist<char,int>>);
    static_assert(std::is_same_v<reverse_t<lst3>,typelist<double,char,int>>);
} // unit_test_reverse

} // reverse_alt_impl
