#pragma once

#include "typelist.hpp"
#include "most_derived.hpp"
#include "replace.hpp"


template<typename List> struct most_derived_first_impl;

template<template<class...> class List, typename T, typename... Ts>
struct most_derived_first_impl<List<T, Ts...>> {
private:
    using the_most_derived = most_derived_t<List<Ts...>, T>;
    using new_tail = replace_t<List<Ts...>, the_most_derived, T>;
public:
    using type = push_front_t<new_tail, the_most_derived>;
};

// template<template<class...> class List, typename T>
// struct most_derived_first_impl<List<T>> {
//     using type = T;
// };

template<typename List>
struct most_derived_first : most_derived_first_impl<List>
{
};

template<typename List>
using most_derived_first_t = typename most_derived_first_impl<List>::type;

namespace most_derived_first_unit_test
{
    struct A { };
    struct B : A { };
    struct C : B { };
    struct D : C { };

    static_assert(std::is_same_v<most_derived_t<typelist<A, B, A>, A>, B>);
    static_assert(std::is_same_v<
                    most_derived_first_t<typelist<A, B, A>>,
                    typelist<B, A, A>>);
    static_assert(std::is_same_v<
                    most_derived_first_t<typelist<A, A, B>>,
                    typelist<B, A, A>>);
    static_assert(std::is_same_v<
                    most_derived_first_t<typelist<A, B, C>>,
                    typelist<C, B, A>>);
} // most_derived_first_unit_test
