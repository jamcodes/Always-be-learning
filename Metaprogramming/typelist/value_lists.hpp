#pragma once
#include "typelist.hpp"
#include "is_empty.hpp"
#include "accumulate.hpp"
#include "insertion_sort.hpp"


// `compili-time-value` - essentially std::integral_constant
template<typename T, T Value>
struct ct_value
{
    using value_type = T;
    using type = ct_value;
    static constexpr inline value_type value = Value;
    constexpr inline value_type operator()() const noexcept { return value; }
    constexpr inline operator value_type() const noexcept { return value; }
};


namespace simple_usage
{

// using the above definition we can define lists of compiletime-values
using Primes = typelist<ct_value<int,2>, ct_value<int,3>,
                        ct_value<int,5>, ct_value<int,7>,
                        ct_value<int,11>>;

// we could also do computations on such compiletime values and lists
template<typename T, typename U>
struct multiply;

template<typename T, T val1, T val2>
struct multiply<ct_value<T,val1>, ct_value<T,val2>>
{
    using type = ct_value<T,val1 * val2>;
};

template<typename T, typename U>
using multiply_t = typename multiply<T,U>::type;


// given the above definitions we could use accumulate to calculate a product of all the numbers
constexpr auto prod = accumulate_t<Primes, multiply, ct_value<int,1>>::value;

} // namespace simple_usage


namespace improved_using_aliases
{
// A downside to the above simple usage is verbosity, we could help with that using aliases:
template<typename T, T... values>
using ct_typelist = typelist<ct_value<T,values>...>;

using Primes = ct_typelist<int, 2,3,5,7,11>;

} // improved_using_aliases


// Or we could define an independent valuelist:
template<typename T, T... Values>
struct valuelist { };

// Specializations for fundamental typelist operations - defining those will give us
// algorithm support

template<typename T, T... Values>
struct is_empty<valuelist<T,Values...>>
{
    static constexpr inline bool value = sizeof...(Values) == 0;
};

template<typename T, T Head, T... Tail>
struct front_impl<valuelist<T, Head, Tail...>>
{
    using type = ct_value<T,Head>;
    static constexpr inline T value = Head;
};

// This specialization works for any valuelist-like template,
// however it's impossible to remove the dependency on ct_value, unless we take a step back
// to valuelists defined in terms of ct_values...
// template<template<typename T, T...>class C, typename V, V head, V... tail>
// struct front_impl<C<V,head,tail...>> {
//     using type = ct_value<V,head>;
//     static constexpr inline V value = head;
// };


template<typename T, T Head, T... Tail>
struct pop_front_impl<valuelist<T,Head,Tail...>>
{
    using type = valuelist<T,Tail...>;
};

template<typename T, T... Values, T New>
struct push_front_impl<valuelist<T,Values...>, ct_value<T,New>>
{
    using type = valuelist<T,New,Values...>;
};

template<typename T, T... Values, T... NewValues>
struct push_front_impl<valuelist<T,Values...>, valuelist<T,NewValues...>>
{
    using type = valuelist<T,NewValues...,Values...>;
};

template<typename T, T... Values, T New>
struct push_back_impl<valuelist<T,Values...>, ct_value<T,New>>
{
    using type = valuelist<T,Values...,New>;
};


template<typename T, typename U>
struct greater_than;

template<typename T, T first, T second>
struct greater_than<ct_value<T,first>, ct_value<T,second>>
{
    static constexpr inline bool value = first > second;
};

using integers = valuelist<int, 6, 2, 4, 9, 5, 2, 1, 7>;
using sorted_integers = insertion_sort_t<integers, greater_than>;
static_assert(std::is_same_v<sorted_integers,valuelist<int,9,7,6,5,4,2,2,1>>);
