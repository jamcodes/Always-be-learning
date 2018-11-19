#pragma once

#include <type_traits>
#include <utility>


/* typelist */
/* --------------------------------------------------------------------------------------------- */
using size_type = std::size_t;

template<size_type Idx>
using Index_constant = std::integral_constant<size_type, Idx>;

template<size_type... Idxs>
using Index_sequence = std::integer_sequence<size_type, Idxs...>;

template<size_type N>
using Make_index_sequence = std::make_integer_sequence<size_type, N>;

template<typename... Ts>
using Index_sequence_for = Make_index_sequence<sizeof...(Ts)>;


template<typename...> struct typelist { };

template<typename List, typename T, size_type N = 0>
struct state_index { };

template<template<class...>class List, typename U, typename T, typename... Ts, size_type Idx>
struct state_index<List<T,Ts...>, U, Idx>
    : std::conditional_t<std::is_same_v<T,U>,
                         Index_constant<Idx>,
                         state_index<List<Ts...>, U, Idx+1>>
{
};

template<typename List, typename T, size_type Idx = 0>
constexpr inline auto state_index_v{state_index<List, T, Idx>::value};
/* --------------------------------------------------------------------------------------------- */


/* traits */
/* --------------------------------------------------------------------------------------------- */
template<typename...> using void_t = void;
template<typename T> struct identity { using type = T; };
template<typename T> struct initial_state { };
template<typename T> constexpr inline auto initial_state_v{initial_state<T>{}};

template<typename...> struct always_false : std::false_type { };
template<typename... Ts> constexpr inline auto always_false_v{always_false<Ts...>::value};

// template<typename T> struct get_state_list { };
// template<template<class...>class FsmT, typename... States>
// struct get_state_list<FsmT<States...>> { using type = typelist<States...>; };
// template<typename T>
// using get_state_list_t = typename get_state_list<T>::type;

template<typename T> struct get_state_list { };
template<template<class,class...>class FsmT, typename T, typename... States>
struct get_state_list<FsmT<T, States...>> { using type = typelist<States...>; };
template<typename T>
using get_state_list_t = typename get_state_list<T>::type;

template<typename State, typename Void, typename... Args>
struct has_handle_event_impl : std::false_type { };
template<typename State, typename... Args>
struct has_handle_event_impl<State,
    void_t<decltype(State::handle_event(std::declval<Args>()...))>,
    Args...>
    : std::true_type
{
};
template<typename State, typename... Args>
struct has_handle_event : has_handle_event_impl<State, void, Args...> { };
template<typename State, typename... Args>
constexpr inline auto has_handle_event_v{has_handle_event<State,Args...>::value};


template<typename T, typename = void_t<>>
struct has_action : std::false_type { };
template<typename T>
struct has_action<T, void_t<decltype(T::action)>> : std::true_type { };
template<typename T>
constexpr inline auto has_action_v{has_action<T>::value};
template<typename T, typename... Args>
struct is_valid_action : std::is_invocable_r<void, T, Args...> { };
template<typename T, typename... Args>
constexpr inline auto is_valid_action_v{is_valid_action<T,Args...>::value};

template<typename T, typename = void_t<>>
struct has_guard : std::false_type { };
template<typename T>
struct has_guard<T, void_t<decltype(T::guard)>> : std::true_type { };
template<typename T>
constexpr inline auto has_guard_v{has_guard<T>::value};
template<typename T, typename... Args>
struct is_valid_guard : std::is_invocable_r<bool, T, Args...> { };
template<typename T, typename... Args>
constexpr inline auto is_valid_guard_v{is_valid_guard<T,Args...>::value};

template<typename T, typename = void_t<>>
struct has_next_state : std::false_type { };
template<typename T>
struct has_next_state<T, void_t<typename T::next_state>> : std::true_type { };
template<typename T>
constexpr inline auto has_next_state_v{has_next_state<T>::value};

template<typename T, bool = has_next_state_v<T>>
struct Next_stateT { };
template<typename Traits>
struct Next_stateT<Traits, true> { using type = typename Traits::next_state; };
template<typename Traits>
using Next_state = typename Next_stateT<Traits>::type;

template<typename T, typename FsmT, typename = void_t<>>
struct has_exit : std::false_type { };
template<template<typename,typename>class Traits, typename FsmT, typename State, typename Event>
struct has_exit<Traits<State,Event>, FsmT,
    void_t<decltype(State::exit(std::declval<FsmT>()))>>
    : std::true_type { };
template<typename T, typename FsmT>
constexpr inline auto has_exit_v{has_exit<T,FsmT>::value};

template<typename T, typename FsmT, typename = void_t<>>
struct has_entry : std::false_type { };
template<template<class,class>class Traits, typename FsmT, typename State, typename Event>
struct has_entry<Traits<State,Event>, FsmT,
    void_t<decltype(State::entry(std::declval<FsmT>()))>>
    : std::true_type { };
template<typename T, typename FsmT>
constexpr inline auto has_entry_v{has_entry<T,FsmT>::value};
/* --------------------------------------------------------------------------------------------- */
