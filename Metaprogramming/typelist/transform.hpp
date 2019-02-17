#pragma once

#include "typelist.hpp"
#include "is_empty.hpp"


template<typename List, template<typename...>class MetaF>
struct transform_impl;

template<template<typename...>class List, template<typename>class MetaF, typename... Ts>
struct transform_impl<List<Ts...>, MetaF>
{
    using type = List<typename MetaF<Ts>::type...>;
};

template<typename List, template<typename>class MetaF>
struct transform : transform_impl<List,MetaF> { };

template<typename List, template<typename>class MetaF>
using transform_t = typename transform<List,MetaF>::type;

namespace unit_test_transform
{

using std::add_const;

using lst0 = typelist<>;
using lst1 = typelist<int>;
using lst2 = typelist<int, char>;
using lst3 = typelist<int, char, double>;
static_assert(std::is_same_v<transform_t<lst0,add_const>,lst0>);
static_assert(std::is_same_v<transform_t<lst1,add_const>,typelist<const int>>);
static_assert(std::is_same_v<transform_t<lst2,add_const>,typelist<const int, const char>>);
static_assert(std::is_same_v<transform_t<lst3,add_const>,
                             typelist<const int, const char, const double>>);

} // namespace unit_test_transform


// ------------------------------------------------------------------------------------------------
namespace transform_alt_impl
{

template<typename List, template<typename T> class MetaFun, bool = is_empty_v<List>>
struct transform;

// recursive case
template<typename List, template<typename T> class MetaFun>
struct transform<List,MetaFun,false>
    : push_front<typename transform<pop_front_t<List>,MetaFun>::type,
                 typename MetaFun<front_t<List>>::type>
{
};

// basis case
template<typename List, template<typename T> class MetaFun>
struct transform<List,MetaFun,true>
{
    using type = List;
};

template<typename List, template<typename T> class MetaFun>
using transform_t = typename transform<List,MetaFun>::type;


namespace unit_test_transform
{

using std::add_const;

using lst0 = typelist<>;
using lst1 = typelist<int>;
using lst2 = typelist<int, char>;
using lst3 = typelist<int, char, double>;
static_assert(std::is_same_v<transform_t<lst0,add_const>,lst0>);
static_assert(std::is_same_v<transform_t<lst1,add_const>,typelist<const int>>);
static_assert(std::is_same_v<transform_t<lst2,add_const>,typelist<const int, const char>>);
static_assert(std::is_same_v<transform_t<lst3,add_const>,
                             typelist<const int, const char, const double>>);

} // unit_test_transform

} // transform_alt_impl
// ------------------------------------------------------------------------------------------------
