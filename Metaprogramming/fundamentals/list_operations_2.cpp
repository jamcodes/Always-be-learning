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

}
