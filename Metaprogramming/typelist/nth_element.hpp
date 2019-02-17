#pragma once
#include "typelist.hpp"


// nth_element - get the nth element of a typelist

template<typename List, unsigned N> struct nth_element_impl;

template<template<typename...> class List, unsigned N, typename T, typename... Ts>
struct nth_element_impl<List<T,Ts...>, N> : nth_element_impl<List<Ts...>, N-1>
{
};

template<template<typename...> class List, typename T, typename... Ts>
struct nth_element_impl<List<T,Ts...>, 0> {
    using type = T;
};

template<typename List, unsigned N>
struct nth_element : nth_element_impl<List,N>
{
};

template<typename List, unsigned N>
using nth_element_t = typename nth_element_impl<List, N>::type;

namespace unit_test_nth_element
{
    using lst_n3 = typelist<int, double, char>;
    static_assert(std::is_same_v<nth_element_t<lst_n3,0>,int>);
    static_assert(std::is_same_v<nth_element_t<lst_n3,1>,double>);
    static_assert(std::is_same_v<nth_element_t<lst_n3,2>,char>);
    // static_assert(std::is_same_v<nth_element_t<lst_n3,100>,void>);   // compile error - GOOD
} // namespace unit_test_nth_element


namespace nth_element_alt_impl
{

// recursive case:
template<typename List, unsigned N>
struct nth_element : nth_element<pop_front_t<List>, N-1> { };

// basic case
template<typename List>
struct nth_element<List, 0> : front<List> { };

template<typename List, unsigned N>
using nth_element_t = typename nth_element<List,N>::type;


// unit tests
namespace unit_test_nth_element
{
    using lst_n3 = typelist<int, double, char>;
    static_assert(std::is_same_v<nth_element_alt_impl::nth_element_t<lst_n3,0>,int>);
    static_assert(std::is_same_v<nth_element_alt_impl::nth_element_t<lst_n3,1>,double>);
    static_assert(std::is_same_v<nth_element_alt_impl::nth_element_t<lst_n3,2>,char>);
    // static_assert(std::is_same_v<nth_element_t<lst_n3,100>,void>);   // compile error - GOOD
} // namespace unit_test_nth_element

} // namespace nth_element_alt_impl
