#pragma once

#include "typelist.hpp"
#include <type_traits>
#include "is_empty.hpp"
#include "identity.hpp"
#include "ifthenelse.hpp"


namespace insertion_sort_alt_impl
{

template<typename List,
         typename Element,
         template<typename, typename> class Compare>
struct insert_sorted_impl;

template<template<typename...>class List,
         typename Element,
         template<typename, typename> class Compare,
         typename T,
         typename... Ts>
struct insert_sorted_impl<List<T, Ts...>, Element, Compare>
{
private:
    using new_tail =
        typename if_then_else_t<Compare<Element, T>::value,
                                identity<List<T, Ts...>>,
                                insert_sorted_impl<List<Ts...>, Element, Compare>
                                >::type;

    using new_head = if_then_else_t<Compare<Element, T>::value,
                                    Element,
                                    T>;
public:
    using type = push_front_t<new_tail, new_head>;
};

template<template<typename...> class List,
         typename Element,
         template<typename, typename> class Compare>
struct insert_sorted_impl<List<>, Element, Compare>
    : push_front<List<>,Element>
{
};

template<typename List,
         typename Element,
         template<typename, typename> class Compare>
struct insert_sorted : insert_sorted_impl<List, Element, Compare> { };

template<typename List, typename Element,
         template<typename,typename>class Compare>
using insert_sorted_t = typename insert_sorted_impl<List,Element,Compare>::type;


template<typename List,
         template<typename,typename>class Compare>
struct insertion_sort_impl;

template<typename List,
         template<typename,typename>class Compare>
using insertion_sort_t = typename insertion_sort_impl<List,Compare>::type;

template<template<typename...>class List,
         template<typename,typename>class Compare,
         typename T, typename... Ts>
struct insertion_sort_impl<List<T,Ts...>,Compare>
    : insert_sorted<insertion_sort_t<List<Ts...>, Compare>, T, Compare> { };

template<template<typename...>class List,
         template<typename,typename>class Compare>
struct insertion_sort_impl<List<>,Compare>
{
    using type = List<>;
};

template<typename List,
         template<typename,typename>class Compare>
struct insertion_sort : insertion_sort_impl<List,Compare> { };

// template<typename List,
//          template<typename,typename>class Compare>
// using insertion_sort_t = typename insertion_sort_impl<List,Compare>::type;


namespace insertion_sort_unit_test
{
template<typename T, typename U>
struct smaller_than {
    static inline constexpr bool value = sizeof(T) < sizeof(U);
};

using lst = typelist<int,short,char,double>;
using lst_sorted = insertion_sort_t<lst, smaller_than>;
static_assert(std::is_same_v<lst_sorted, typelist<char,short,int,double>>);
}
} // namespace insertion_sort_alt_impl

// namespace insertion_sort_alt_impl
// {

template<typename List, typename Element,
         template<typename,typename> class Compare,
         bool = is_empty_v<List>>
struct insert_sorted;

template<typename List, typename Element,
         template<typename,typename> class Compare>
struct insert_sorted<List,Element,Compare,false>
{
private:
    // compute the tail of the resulting list:
    using new_tail =
        typename std::conditional_t<Compare<Element, front_t<List>>::value,
                                  identity<List>,
                                  insert_sorted<pop_front_t<List>,Element,Compare>
                                  >::type;

    // compute the head of the resulting list:
    using new_head = std::conditional_t<Compare<Element,front_t<List>>::value,
                                      Element,
                                      front_t<List>>;
public:
    using type = push_front_t<new_tail, new_head>;
};

// basis case:
template<typename List, typename Element,
         template<typename,typename> class Compare>
struct insert_sorted<List,Element,Compare,true>
    : push_front<List, Element>
{
};

template<typename List, typename Element,
         template<typename,typename> class Compare>
using insert_sorted_t = typename insert_sorted<List,Element,Compare>::type;



template<typename List,
         template<typename T, typename U> class Compare,
         bool = is_empty_v<List>>
class insertion_sort;

template<typename List,
         template<typename T, typename U> class Compare>
using insertion_sort_t = typename insertion_sort<List,Compare>::type;

// recursive case (insert first element into sorted list):
template<typename List,
         template<typename,typename> class Compare>
struct insertion_sort<List, Compare, false>
    : insert_sorted<insertion_sort_t<pop_front_t<List>, Compare>,
                    front_t<List>, Compare>
{
};

// basis case (an empty list is sorted):
template<typename List,
         template<typename,typename> class Compare>
struct insertion_sort<List,Compare,true>
{
    using type = List;
};


namespace unit_test_insertion_sort
{

template<typename T, typename U>
struct smaller_than {
    static inline constexpr bool value = sizeof(T) < sizeof(U);
};

using lst = typelist<int,short,char,double>;
using lst_sorted = insertion_sort_t<lst, smaller_than>;
static_assert(std::is_same_v<lst_sorted, typelist<char,short,int,double>>);

} // unit_test_insertion_sort

// } // insertion_sort_alt_impl

