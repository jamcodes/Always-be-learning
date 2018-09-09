#pragma once


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
namespace
{
    template<typename... Ts>
    struct back : back_impl<Ts...> { };
}

template<typename... Ts>
using back = typename back_impl<Ts...>::type;
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
namespace lazy
{
    template<typename... Ts>
    struct front : front_impl<Ts...> { };
}

template<typename... Ts>
using front = typename front_impl<Ts...>::type;
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
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


// capacity
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
template<typename...> struct has_tail;

template<typename T>
struct has_tail<T> : std::false_type { };

template<typename T, typename... Ts>
struct has_tail<T,Ts...> : std::true_type { };

template< template<class> class C, typename T>
struct has_tail<C<T>> : std::false_type { };

template< template<class...> class C, typename T, typename... Ts>
struct has_tail<C<T, Ts...>> : std::true_type { };

template<typename List>
constexpr inline auto has_tail_v{has_tail<List>::value};

// count
template<typename... Ts>
struct count : std::make_index_sequence<sizeof...(Ts)> { };

template<template<class...> class C, typename... Ts>
struct count<C<Ts...>> : count<Ts...> { };

template<typename List>
constexpr inline auto count_v{count<List>::value};
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
