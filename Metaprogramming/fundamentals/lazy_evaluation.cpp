#include <iostream>
#include <type_traits>


/**
 * A 'trick' can be used to potentially speed up compilation by lazily evaluating templates.
 * It's achieved by separating the implementation details and the interface, possibly in different
 * namespaces (not necessary?).
 */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
template<typename... Ts>
struct list { };
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


// last element of the list
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
template<typename...> struct back_impl;

template<typename T>
struct back_impl<T> { using type = T; };

template<typename T, typename... Ts>
struct back_impl<T,Ts...> { using type = typename back_impl<Ts...>::type; };

template<template<class> class C, typename T>
struct back_impl<C<T>> { using type = T; };    // the only element is the last element

template<template<class...> class C, typename Head, typename... Tail>
struct back_impl<C<Head,Tail...>> { using type = typename back_impl<Tail...>::type; };


template<typename... Ts>
struct back : back_impl<Ts...> { };

template<typename... Ts>
using back_t = typename back<Ts...>::type;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


// first element of a list
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
template<typename...> struct front_impl { };
template<template<class...> class C, typename T, typename... Ts>
struct front_impl<C<T, Ts...>> { using type = T; };

template<typename T, typename... Ts>
struct front_impl<T,Ts...> { using type = T; };

template<typename... Ts>
struct front : front_impl<Ts...> { };

template<typename... Ts>
using front_t = typename front<Ts...>::type;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


int main()
{
    using stuff = list<int, char, double>;

    static_assert(sizeof(back_t<stuff>) == sizeof(double), "back didn't return double");
    static_assert(sizeof(back_t<list<double>>) == sizeof(double), "...");
    static_assert(sizeof(back_t<double>) == sizeof(double), "...");
    static_assert(sizeof(back_t<list<double,int>>) == sizeof(int), "...");
    static_assert(sizeof(back_t<double, float>) == sizeof(float), "...");

    static_assert(sizeof(front_t<stuff>) == sizeof(int), "front_t didn't return int");
    static_assert(sizeof(front_t<list<double>>) == sizeof(double), "...");
    static_assert(sizeof(front_t<double>) == sizeof(double), "...");
    static_assert(sizeof(front_t<double, int>) == sizeof(double), "...");
    static_assert(sizeof(front_t<list<double,int>>) == sizeof(double), "...");

}
