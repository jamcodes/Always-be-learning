#pragma once

#include <type_traits>
#include "typelist.hpp"
#include "is_empty.hpp"
#include "ifthenelse.hpp"
#include "typelist_algorithm.hpp"


template<typename List, typename T, unsigned N = 0> struct find_index_of_impl;

template<template<typename...> class List, typename T, unsigned N, typename U, typename... Us>
struct find_index_of_impl<List<U, Us...>, T, N>
    : if_then_else_t<std::is_same_v<T, U>,
                   std::integral_constant<unsigned, N>,
                   find_index_of_impl<List<Us...>, T, N+1>>
{
};

template<typename T>
struct type_not_found { static_assert(sizeof(T) == 0, ""); };

template<template<typename...> class List, typename T, unsigned N>
struct find_index_of_impl<List<>, T, N> : type_not_found<T>
{
};

template<typename List, typename T>
struct find_index_of : find_index_of_impl<List, T>
{
};

template<typename List, typename T>
constexpr inline auto find_index_of_v{find_index_of_impl<List,T>::value};

namespace find_index_of_unit_test
{
using lst0 = typelist<>;
using lst1 = typelist<int>;
using lst2 = typelist<int, double>;
using lst3 = typelist<double, char, int>;

// static_assert(find_index_of_v<lst0, int> == 0, "type not found");
static_assert(find_index_of_v<lst1, int> == 0);
static_assert(find_index_of_v<lst2, int> == 0);
static_assert(find_index_of_v<lst2, double> == 1);
static_assert(find_index_of_v<lst3, double> == 0);
static_assert(find_index_of_v<lst3, char> == 1);
static_assert(find_index_of_v<lst3, int> == 2);
} // namespace find_index_of_unit_test


namespace find_index_of_alt_impl
{

template<typename List, typename T, unsigned N = 0,
         bool = is_empty_v<List>>
struct find_index_of;

template<typename List, typename T, unsigned N>
struct find_index_of<List, T, N, false>
    : public if_then_else_t<std::is_same_v<front_t<List>, T>,
                            std::integral_constant<unsigned, N>,
                            find_index_of<pop_front_t<List>, T, N+1>>
{
};

template<typename List, typename T, unsigned N>
struct find_index_of<List, T, N, true>
{
};

template<typename List, typename T, unsigned N>
constexpr inline auto find_index_of_v = find_index_of<List,T,N>::value;

} // namespace find_index_of_alt_impl
