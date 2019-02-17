#pragma once

#include "typelist.hpp"
#include "ifthenelse.hpp"
#include "identity.hpp"
#include "transform.hpp"
#include <type_traits>


template<typename List, typename S, typename R> struct replace_impl;

template<template<typename...> class List, typename S, typename R, typename... Ts>
struct replace_impl<List<Ts...>, S, R> {
    using type = List<if_then_else_t<std::is_same_v<Ts, S>, R, Ts>...>;
};



template<typename List, typename S, typename R>
struct replace : replace_impl<List, S, R>
{
};

template<typename List, typename S, typename R>
using replace_t = typename replace<List, S, R>::type;

namespace replace_unit_test
{
    using lst1 = typelist<int>;
    using lst2 = typelist<int, double>;
    using lst3 = typelist<int, double, char, double>;
    static_assert(std::is_same_v<replace_t<lst1, int, char>, typelist<char>>);
    static_assert(std::is_same_v<replace_t<lst1, double, char>, lst1>);
    static_assert(std::is_same_v<replace_t<lst2, int, double>, typelist<double, double>>);
    static_assert(std::is_same_v<replace_t<lst3, double, float>, typelist<int, float, char, float>>);
}


namespace replace_alt_impl
{

template<typename List, typename S, typename R> struct replace_impl;

template<template<typename...> class List,
         typename S, typename R,
         typename T, typename... Ts>
struct replace_impl<List<T, Ts...>, S, R> {
private:
    using tail = typename replace_impl<List<Ts...>, S, R>::type;
public:
    using type = push_front_t<tail, if_then_else_t<std::is_same_v<T, S>, R, T>>;
};

template<template<typename...> class List,
         typename S, typename R,
         typename T>
struct replace_impl<List<T>, S, R> {
    using type = List<if_then_else_t<std::is_same_v<T, S>, R, T>>;
};

template<template<typename...> class List,
         typename S, typename R>
struct replace_impl<List<>, S, R>
{
};

template<typename List, typename S, typename R>
struct replace : replace_impl<List, S, R>
{
};

template<typename List, typename S, typename R>
using replace_t = typename replace<List, S, R>::type;

    using lst1 = typelist<int>;
    using lst2 = typelist<int, double>;
    using lst3 = typelist<int, double, char, double>;
    static_assert(std::is_same_v<replace_t<lst1, int, char>, typelist<char>>);
    static_assert(std::is_same_v<replace_t<lst1, double, char>, lst1>);
    static_assert(std::is_same_v<replace_t<lst2, int, double>, typelist<double, double>>);
    static_assert(std::is_same_v<replace_t<lst3, double, float>, typelist<int, float, char, float>>);

} // replace_alt_impl


namespace replace_using_transform
{

template<typename S, typename R> struct replace_cmp_impl {
    template<typename T>
    struct cmp {
        using type = if_then_else_t<std::is_same_v<T,S>, R, T>;
    };
};

// not valid?
// template<typename S, typename R>
// using replace_cmp = typename replace_cmp_impl<S,R>::cmp;

// this?
// template<typename S, typename R>
// using replace_cmp = replace_cmp_impl<S,R>::template cmp;

// or this?
// template<typename S, typename R>
// using replace_cmp = typename replace_cmp_impl<S,R>::template cmp;

// template<typename List, typename S, typename R>
// using replace_t = transform_t<List, replace_cmp<S,R>>;

template<typename List, typename S, typename R>
using replace_t = transform_t<List, replace_cmp_impl<S,R>::template cmp>;

    using lst1 = typelist<int>;
    using lst2 = typelist<int, double>;
    using lst3 = typelist<int, double, char, double>;
    static_assert(std::is_same_v<replace_t<lst1, int, char>, typelist<char>>);
    static_assert(std::is_same_v<replace_t<lst1, double, char>, lst1>);
    static_assert(std::is_same_v<replace_t<lst2, int, double>, typelist<double, double>>);

} // namespace replace_using_transform
