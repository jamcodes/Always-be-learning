#pragma once

#include "typelist.hpp"
#include "ifthenelse.hpp"
#include "is_derived.hpp"
#include <type_traits>


template<typename List, typename Base> struct most_derived_impl;

template<template<typename...> class List,
         typename Base,
         typename T, typename... Ts>
struct most_derived_impl<List<T, Ts...>, Base> {
private:
    using candidate = typename most_derived_impl<List<Ts...>, Base>::type;
public:
    using type = if_then_else_t<is_derived_from_v<candidate, T>,
                                candidate,
                                T>;
};

template<template<typename...> class List,
         typename Base,
         typename T>
struct most_derived_impl<List<T>, Base>
    : if_then_else<is_derived_from_v<T, Base>,
                   T,
                   Base>
{
    // using type = if_then_else_t<std::is_base_of_v<Base, T>,
    //                              T, Base>;
};


// template<template<typename...> class List, typename Base>
// struct most_derived_impl<List<>, Base> {
//     using type = Base;
// };

template<typename List, typename Base>
struct most_derived : most_derived_impl<List, Base>
{
};

template<typename List, typename Base>
using most_derived_t = typename most_derived_impl<List, Base>::type;

namespace most_derived_unit_test
{
    struct A { };
    struct B : A { };
    struct C : B { };

    static_assert(std::is_same_v<most_derived_t<typelist<A>, A>, A>);
    static_assert(std::is_same_v<most_derived_t<typelist<A, int, A>, A>, A>);
    static_assert(std::is_same_v<most_derived_t<typelist<A, A, B>, A>, B>);
    static_assert(std::is_same_v<most_derived_t<typelist<A, B, A>, A>, B>);
    static_assert(std::is_same_v<most_derived_t<typelist<B, A, A>, A>, B>);
    static_assert(std::is_same_v<most_derived_t<typelist<C, B, A>, C>, C>);
} // most_derived_unit_test
