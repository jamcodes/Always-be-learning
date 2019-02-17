#pragma once

#include "typelist.hpp"
#include "is_empty.hpp"
#include "ifthenelse.hpp"
#include "reverse.hpp"
#include "largest_type.hpp"


template<typename List, template<typename,typename>class F, typename Init>
struct accumulate_impl;

template<template<typename...>class List,
         template<typename, typename> class F,
         typename Init,
         typename T, typename... Ts>
struct accumulate_impl<List<T, Ts...>, F, Init>
    : accumulate_impl<List<Ts...>, F, typename F<Init, T>::type>
{
};

template<template<typename...>class List,
         template<typename, typename> class F,
         typename Init,
         typename T>
struct accumulate_impl<List<T>, F, Init> {
    using type = typename F<Init,T>::type;
};

template<template<typename...> class List,
         template<typename, typename> class F,
         typename Init>
struct accumulate_impl<List<>,F,Init>
{
    using type = Init;
};

template<typename List,
         template<typename, typename> class F,
         typename Init>
struct accumulate : accumulate_impl<List, F, Init>
{
};

template<typename List,
         template<typename, typename> class F,
         typename Init>
using accumulate_t = typename accumulate_impl<List,F,Init>::type;

namespace accumulate_unit_test
{
using lst0 = typelist<>;
using lst1 = typelist<int>;
using lst2 = typelist<int, char>;
using lst3 = typelist<int, char, double>;

static_assert(std::is_same_v<accumulate_t<lst3,a::push_front,lst0>,
                             reverse_t<lst3>>);

template<bool B, typename T, typename U> struct if_then_else { using type = T; };
template<typename T, typename U> struct if_then_else<false,T,U> { using type = U; };

template<typename T, typename U>
struct larger_type : if_then_else<sizeof(U) < sizeof(T), T, U> { };
template<typename T, typename U> using larger_type_t = typename larger_type<T,U>::type;
static_assert(std::is_same_v<larger_type_t<char,double>,double>);

static_assert(std::is_same_v<accumulate_t<pop_front_t<lst3>,larger_type,front_t<lst3>>,
                             largest_type_t<lst3>>);
} // namespace accumulate_unit_test


namespace accumulate_alt_impl
{

template<typename List,
         template<typename X, typename Y> class F,
         typename I,
         bool = is_empty_v<List>>
struct accumulate;

// recursive case
template<typename List,
         template<typename X, typename Y> class F,
         typename I>
struct accumulate<List, F, I, false>
    : public accumulate<pop_front_t<List>, F, typename F<I, front_t<List>>::type>
{
};

// basis case
template<typename List,
         template<typename X, typename Y> class F,
         typename I>
struct accumulate<List, F, I, true> {
    using type = I;
};

template<typename List,
         template<typename X, typename Y> class F,
         typename I>
using accumulate_t = typename accumulate<List, F, I>::type;


namespace unit_test_accumulate
{
using lst0 = typelist<>;
using lst1 = typelist<int>;
using lst2 = typelist<int, char>;
using lst3 = typelist<int, char, double>;

static_assert(std::is_same_v<accumulate_t<lst3,a::push_front,lst0>,
                             reverse_t<lst3>>);

template<typename T, typename U>
struct larger_type : if_then_else<sizeof(U) < sizeof(T), T, U> { };
template<typename T, typename U> using larger_type_t = typename larger_type<T,U>::type;
static_assert(std::is_same_v<larger_type_t<char,double>,double>);

static_assert(std::is_same_v<accumulate_t<pop_front_t<lst3>,larger_type,front_t<lst3>>,
                             largest_type_t<lst3>>);
} // unit_test_accumulate

} // accumulate_alt_impl
