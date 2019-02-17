#pragma once
#include <type_traits>
#include "typelist.hpp"
#include "is_empty.hpp"


template<typename List> struct largest_type_impl;

template<template<typename...>class List, typename T, typename... Ts>
struct largest_type_impl<List<T,Ts...>>
{
private:
    using tail = typename largest_type_impl<List<Ts...>>::type;
public:
    using type = std::conditional_t<sizeof(T) >= sizeof(tail), T, tail>;
};

template<template<typename...>class List, typename T>
struct largest_type_impl<List<T>>
{
    using type = T;
};

template<template<typename...>class List>
struct largest_type_impl<List<>>
{
    using type = char;
};

template<typename List>
struct largest_type : largest_type_impl<List> { };

template<typename List>
using largest_type_t = typename largest_type<List>::type;


namespace largets_type_unit_test
{
using lst0 = typelist<>;
static_assert(std::is_same_v<largest_type_t<lst0>,char>);
using lst1 = typelist<void*>;
static_assert(std::is_same_v<largest_type_t<lst1>,void*>);
using lst3 = typelist<int, double, char>;
static_assert(std::is_same_v<largest_type_t<lst3>,double>);
} // namespace largest_type_unit_test


namespace largest_type_alt_impl
{

template<typename List, bool = is_empty_v<List>>
struct largest_type;

template<typename List>
struct largest_type<List,false>
{
private:
    using head = front_t<List>;
    using tail = typename largest_type<pop_front_t<List>>::type;
public:
    using type = std::conditional_t<(sizeof(head) >= sizeof(tail)), head, tail>;
};

// basis case
template<typename List>
struct largest_type<List,true>
{
    using type = char;
};

template<typename List>
using largest_type_t = typename largest_type<List>::type;


// unit tests
namespace unit_test_largest_type
{

using lst0 = typelist<>;
static_assert(std::is_same_v<largest_type_t<lst0>,char>);
using lst1 = typelist<void*>;
static_assert(std::is_same_v<largest_type_t<lst1>,void*>);
using lst3 = typelist<int, double, char>;
static_assert(std::is_same_v<largest_type_t<lst3>,double>);

} // namespace unit_test_largest_type

} // namespace larest_type_alt_impl
