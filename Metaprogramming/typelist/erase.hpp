#pragma once

#include "typelist.hpp"
#include "identity.hpp"

template<typename List, typename T> struct erase_impl;

template<template<typename...> class List, typename T, typename U, typename... Us>
struct erase_impl<List<U, Us...>, T> {
    using type = typename if_then_else_t<std::is_same_v<T, U>,
                                identity<List<Us...>>,
                                push_front<typename erase_impl<List<Us...>, T>::type, U>
                                >::type;
};

template<template<typename...> class List, typename T, typename U>
struct erase_impl<List<U>, T> {
    using type = if_then_else_t<std::is_same_v<T, U>,
                            List<>,
                            List<U>
                            >;
};

template<typename List, typename T>
struct erase : erase_impl<List, T>
{
};

template<typename List, typename T>
using erase_t = typename erase_impl<List, T>::type;

namespace erase_unit_test
{

    using lst1 = typelist<int>;
    using lst2 = typelist<char, int>;
    using lst3 = typelist<double, int, double>;
    static_assert(std::is_same_v<erase_t<lst1, int>, typelist<>>);
    static_assert(std::is_same_v<erase_t<lst1, char>, lst1>);
    static_assert(std::is_same_v<erase_t<lst2, char>, typelist<int>>);
    static_assert(std::is_same_v<erase_t<lst2, int>, typelist<char>>);
    static_assert(std::is_same_v<erase_t<lst3, double>, typelist<int, double>>);
    static_assert(std::is_same_v<erase_t<lst3, int>, typelist<double, double>>);

} // namespace erase_all_unit_test


template<typename List, typename T> struct erase_all_impl;

template<template<typename...> class List, typename T, typename U, typename... Us>
struct erase_all_impl<List<U,Us...>, T> {
private:
    using tail = typename erase_all_impl<List<Us...>, T>::type;
public:
    using type = if_then_else_t<std::is_same_v<T,U>,
                                tail,
                                push_front_t<tail, U>
                                >;
};

template<template<typename...> class List, typename T, typename U>
struct erase_all_impl<List<U>, T> {
    using type = if_then_else_t<std::is_same_v<T, U>, List<>, List<U>>;
};

template<typename List, typename T>
struct erase_all : erase_all_impl<List, T>
{
};

template<typename List, typename T>
using erase_all_t = typename erase_all_impl<List, T>::type;


namespace erase_all_unit_test
{

    using lst1 = typelist<int>;
    using lst2 = typelist<char, int>;
    using lst3 = typelist<double, int, double>;
    static_assert(std::is_same_v<erase_all_t<lst1, int>, typelist<>>);
    static_assert(std::is_same_v<erase_all_t<lst1, char>, lst1>);
    static_assert(std::is_same_v<erase_all_t<lst2, char>, typelist<int>>);
    static_assert(std::is_same_v<erase_all_t<lst2, int>, typelist<char>>);
    static_assert(std::is_same_v<erase_all_t<lst3, double>, typelist<int>>);
    static_assert(std::is_same_v<erase_all_t<lst3, int>, typelist<double, double>>);

} // namespace erase_all_unit_test
