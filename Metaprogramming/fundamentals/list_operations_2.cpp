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

// support for raw element packs - back<int, double, ...> etc.
template<typename T>
struct back_impl<T> { using type = T; };
// support for raw element packs
template<typename T, typename... Ts>
struct back_impl<T,Ts...> { using type = typename back_impl<Ts...>::type; };

// support for types holding element packs - back<list<int, double, ...>> etc.
template<template<class> class C, typename T>
struct back_impl<C<T>> { using type = T; };    // the only element is the last element

// support for types holding element packs
template<template<class...> class C, typename Head, typename... Tail>
struct back_impl<C<Head,Tail...>> { using type = typename back_impl<C<Tail...>>::type; };


// public interface
template<typename... Ts>
struct back : back_impl<Ts...> { };

template<typename... Ts>
using back_t = typename back<Ts...>::type;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


// first element of a list
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
template<typename...> struct front_impl;

// support for raw element packs - front<int, double, ...> etc.
template<typename T, typename... Ts>
struct front_impl<T,Ts...> { using type = T; };

// support for types holding element packs - front<list<int, double, ...>> etc.
template<template<class...> class C, typename T, typename... Ts>
struct front_impl<C<T, Ts...>> { using type = T; };

// public interface
template<typename... Ts>
struct front : front_impl<Ts...> { };

template<typename... Ts>
using front_t = typename front<Ts...>::type;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


// Adding elements to a list
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
template<typename L, typename... Ts> struct push_front_impl;

template<template<class...> class C, typename... Us, typename... Ts>
struct push_front_impl<C<Ts...>, Us...> {
    using type = C<Us..., Ts...>;
};

namespace lazy
{

template<typename List, typename... Ts>
struct push_front : push_front_impl<List, Ts...> { };

}

template<typename List, typename... Ts>
using push_front = typename push_front_impl<List,Ts...>::type;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

// Removing elements from the list
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
template<typename L> struct pop_front_impl;

template<template<class...> class C, typename T, typename... Ts>
struct pop_front_impl<C<T,Ts...>> {
    using type = C<Ts...>;
};

namespace lazy
{
    template<typename List>
    struct pop_front : pop_front_impl<List> { };
}

template<typename List>
using pop_front = typename pop_front_impl<List>::type;

// TODO: figure out how to pop multiple elements at once
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

int main()
{
    using stuff = list<int, char, double>;
    using two_elem = list<int, float>;
    using one_elem = list<double>;

    static_assert(std::is_same_v<back_t<stuff>,double>,
        "back_t: three elem list case failed");
    static_assert(std::is_same_v<back_t<one_elem>, double>,
        "back_t: single element list case failed");
    static_assert(std::is_same_v<back_t<double>, double>,
        "back_t: single element pack case failed");
    static_assert(std::is_same_v<back_t<two_elem>, float>,
        "back_t: two elem list case failed");
    static_assert(std::is_same_v<back_t<int, float>, float>,
        "back_t: two elem pack failed");

    static_assert(std::is_same_v<front_t<stuff>, int>,
        "front_t: three elem list case failed");
    static_assert(std::is_same_v<front_t<one_elem>, double>,
        "front_t: single elem list case failed");
    static_assert(std::is_same_v<front_t<double>, double>,
        "front_t: single elem pack case failed");
    static_assert(std::is_same_v<front_t<two_elem>, int>, "front_t: two elem list case failed");
    static_assert(std::is_same_v<front_t<float, int>, float>,
        "front_t: two elem pack case failed");


    using lst1 = list<int, char>;
    using lst1_prim = push_front<lst1, double>;
    using lst1_prim_lazy = lazy::push_front<lst1, double>;
    using lst1_expected = list<double, int, char>;
    static_assert(std::is_same_v<lst1_expected, lst1_prim>, "push_front test failed");
    static_assert(std::is_same_v<lst1_expected, typename lst1_prim_lazy::type>,
        "lazy push_front test failed");

    using lst_empty = list<>;
    using lst_empty_prim = push_front<lst_empty, double>;
    using lst_empty_prim_lazy = lazy::push_front<lst_empty, double>;
    using lst_empty_expected = list<double>;
    static_assert(std::is_same_v<lst_empty_expected, lst_empty_prim>,
        "push_front empty list test failed");
    static_assert(std::is_same_v<lst_empty_expected, typename lst_empty_prim_lazy::type>,
        "lazy push_front empty list test failed");

    using lst2 = list<int>;
    using lst2_prim = push_front<lst2, char, double>;
    using lst2_prim_lazy = lazy::push_front<lst2, char, double>;
    using lst2_expected = list<char, double, int>;
    static_assert(std::is_same_v<lst2_expected, lst2_prim>,
        "push_front empty list test failed");
    static_assert(std::is_same_v<lst2_expected, typename lst2_prim_lazy::type>,
        "lazy push_front empty list test failed");


    using lstpop1 = list<int, char>;
    using lstpop1_prim = pop_front<lstpop1>;
    using lstpop1_prim_lazy = lazy::pop_front<lstpop1>;
    using lstpop1_expected = list<char>;
    static_assert(std::is_same_v<lstpop1_prim, lstpop1_expected>, "pop_front test 1 failed");
    static_assert(std::is_same_v<typename lstpop1_prim_lazy::type, lstpop1_expected>,
        "lazy pop_front test 1 failed");

    using lstpop2 = list<int>;
    using lstpop2_prim = pop_front<lstpop2>;
    using lstpop2_prim_lazy = lazy::pop_front<lstpop2>;
    using lstpop2_expected = list<>;
    static_assert(std::is_same_v<lstpop2_prim, lstpop2_expected>, "pop_front test 2 failed");
    static_assert(std::is_same_v<typename lstpop2_prim_lazy::type, lstpop2_expected>,
        "lazy pop_front test 2 failed");
}
