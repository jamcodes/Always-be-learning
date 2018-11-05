#pragma once

#include <type_traits>
#include <utility>

template<typename...> struct always_false : std::false_type { };
template<typename... Ts> constexpr inline auto always_false_v{always_false<Ts...>::value};

template<typename List, typename T, unsigned N = 0>
struct state_index { };

template<template<class...>class List, typename U, typename T, typename... Ts, unsigned N>
struct state_index<List<T, Ts...>, U, N>
    : std::conditional_t<std::is_same_v<T,U>,
                         std::integral_constant<unsigned, N>,
                         state_index<List<Ts...>, U, N+1>>
{
};

template<typename List, typename T, unsigned N = 0>
constexpr inline auto state_index_v{state_index<List,T,N>::value};


template<typename S, typename E>
struct transition_table { static_assert(always_false_v<S,E>, "Unhandled state transition"); };


template<unsigned Index, typename T>
class FsmState
{
private:
    T value_{};
public:
    using type = T;

    constexpr FsmState() = default;

    template<typename U /* ,typename = std::enable_if_t<std::is_convertible_v<U, T>> */>
    constexpr FsmState(U&& value) noexcept(std::is_nothrow_constructible_v<T,U>)
        : value_{std::forward<U>(value)}
        { }

    template<typename Event>
    constexpr void handle_event(Event&& event) noexcept
    {
        value_.handle_event(std::forward<Event>(event));
    }

    constexpr T&& Get() && noexcept { return std::move(value_); }
    constexpr T& Get() & noexcept { return value_; }
    constexpr T const& Get() const& noexcept { return value_; }
};

template<unsigned I, typename T>
using FsmState_t = typename FsmState<I,T>::type;

template<unsigned I, typename U>
inline constexpr FsmState_t<I,U>& Get_impl(FsmState<I,U>& s) noexcept;
template<unsigned I, typename U>
inline constexpr FsmState_t<I,U> const& Get_impl(FsmState<I,U> const& s) noexcept;
template<unsigned I, typename U>
inline constexpr FsmState_t<I,U>&& Get_impl(FsmState<I,U>&& s) noexcept;


template<unsigned... Is>
using id_sequence = std::integer_sequence<unsigned, Is...>;
template<unsigned N>
using make_id_sequence = std::make_integer_sequence<unsigned, N>;


template<typename Indices, typename... States>
class Fsm_impl;

template<unsigned... Is, typename... States>
class Fsm_impl<id_sequence<Is...>, States...> : private FsmState<Is, States>...
{
public:
    constexpr Fsm_impl() noexcept = default;
    template<typename... Ts>
    constexpr Fsm_impl(Ts&&... states) noexcept( (... && std::is_nothrow_constructible_v<States,Ts>) )
        : FsmState<Is,States>{std::forward<Ts>(states)}...
        { }

    template<typename Indices, typename... St, typename Event, unsigned Idx, unsigned... Idxs>
    friend inline void dispatch_event(Fsm_impl<Indices, St...>& fsm, Event&& event,
                                      id_sequence<Idx, Idxs...>) noexcept;

    template<typename Indices, typename... St, typename Event, unsigned Idx, unsigned... Idxs>
    friend inline void dispatch_event(Fsm_impl<Indices, St...> const& fsm, Event&& event,
                                      id_sequence<Idx, Idxs...>) noexcept;

    template<typename Indices, typename... St, typename Event, unsigned Idx, unsigned... Idxs>
    friend inline void dispatch_event(Fsm_impl<Indices, St...>&& fsm, Event&& event,
                                      id_sequence<Idx, Idxs...>) noexcept;

    template<unsigned I, typename Indices, typename... St>
    friend inline constexpr decltype(auto) Get(Fsm_impl<Indices, St...>&) noexcept;
    template<unsigned I, typename Indices, typename... St>
    friend inline constexpr decltype(auto) Get(Fsm_impl<Indices, St...> const&) noexcept;
    template<unsigned I, typename Indices, typename... St>
    friend inline constexpr decltype(auto) Get(Fsm_impl<Indices, St...>&&) noexcept;

    template<unsigned I, typename Event>
    inline void traisition(Event&& event) noexcept;
private:
    unsigned state_{};
};


template<unsigned I, typename U>
inline constexpr FsmState_t<I,U>& Get_impl(FsmState<I,U>& st) noexcept
{
    return st.Get();
}
template<unsigned I, typename U>
inline constexpr FsmState_t<I,U> const& Get_impl(FsmState<I,U> const& st) noexcept
{
    return st.Get();
}
template<unsigned I, typename U>
inline constexpr FsmState_t<I,U>&& Get_impl(FsmState<I,U>&& st) noexcept
{
    return st.Get();
}

template<unsigned I, typename Indices, typename... St>
inline constexpr decltype(auto) Get(Fsm_impl<Indices, St...>& fsm) noexcept
{
    return Get_impl<I>(fsm);
}
template<unsigned I, typename Indices, typename... St>
inline constexpr decltype(auto) Get(Fsm_impl<Indices, St...> const& fsm) noexcept
{
    return Get_impl<I>(fsm);
}
template<unsigned I, typename Indices, typename... St>
inline constexpr decltype(auto) Get(Fsm_impl<Indices, St...>&& fsm) noexcept
{
    return Get_impl<I>(fsm);
}

template<typename...> struct typelist { };


template<unsigned I, typename FsmT>
using at = decltype( Get<I>(std::declval<FsmT>()) );

template<unsigned... Is, typename... States>
  template<unsigned I, typename Event>
inline void Fsm_impl<id_sequence<Is...>, States...>::traisition(Event&& event) noexcept
{
    using E = std::decay_t<Event>;
    using S = std::decay_t<at<I, Fsm_impl<id_sequence<Is...>, States...>>>;
    using next_state = typename transition_table<S,E>::type;
    state_ = state_index_v<typelist<States...>, next_state>;
}


template<unsigned I, typename U, typename Event>
inline constexpr void handle_event(FsmState<I, U>& fs, Event&& event) noexcept
{
    fs.handle_event(std::forward<Event>(event));
}

template<unsigned I, typename U, typename Event>
inline constexpr void handle_event(FsmState<I, U> const& fs, Event&& event) noexcept
{
    fs.handle_event(std::forward<Event>(event));
}

template<unsigned I, typename U, typename Event>
inline constexpr void handle_event(FsmState<I,U>&& fs, Event&& event) noexcept
{
    fs.handle_event(std::forward<Event>(event));
}

template<typename T> struct identity { using type = T; };

template<typename Indices, typename... States, typename Event, unsigned Idx, unsigned... Idxs>
inline void dispatch_event(Fsm_impl<Indices, States...>& fsm, Event&& event, id_sequence<Idx, Idxs...>) noexcept
{
    if (Idx == fsm.state_) {
        // handle_event<Idx>(fsm, std::forward<Event>(event));
        Get<Idx>(fsm).handle_event(std::forward<Event>(event));
        fsm.template traisition<Idx>(std::forward<Event>(event));
    }
    else if constexpr (sizeof...(Idxs) != 0) {
        dispatch_event(fsm, std::forward<Event>(event), id_sequence<Idxs...>{});
    }
    else {
        // static_assert(always_false_v<identity<Event>>, "Fsm in an invalid state");
    }
}

template<typename Indices, typename... States, typename Event, unsigned Idx, unsigned... Idxs>
inline void dispatch_event(Fsm_impl<Indices, States...> const& fsm, Event&& event,
                                    id_sequence<Idx, Idxs...>) noexcept
{
    if (Idx == fsm.state_) {
        // handle_event<Idx>(fsm, std::forward<Event>(event));
        Get<Idx>(fsm).handle_event(std::forward<Event>(event));
        fsm.template traisition<Idx>(std::forward<Event>(event));
    }
    else if constexpr (sizeof...(Idxs) != 0) {
        dispatch_event(fsm, std::forward<Event>(event), id_sequence<Idxs...>{});
    }
    else {
        // static_assert(always_false_v<identity<Event>>, "Fsm in an invalid state");
    }
}

template<typename Indices, typename... States, typename Event, unsigned Idx, unsigned... Idxs>
inline void dispatch_event(Fsm_impl<Indices, States...>&& fsm, Event&& event,
                                    id_sequence<Idx, Idxs...>) noexcept
{
    if (Idx == fsm.state_) {
        // handle_event<Idx>(fsm, std::forward<Event>(event));
        Get<Idx>(fsm).handle_event(std::forward<Event>(event));
        fsm.template traisition<Idx>(std::forward<Event>(event));
    }
    else if constexpr (sizeof...(Idxs) != 0) {
        dispatch_event(fsm, std::forward<Event>(event), id_sequence<Idxs...>{});
    }
    else {
        // static_assert(always_false_v<identity<Event>>, "Fsm in an invalid state");
    }
}


template<typename... States>
class Fsm : public Fsm_impl<make_id_sequence<sizeof...(States)>, States...>
{
    using Ids = make_id_sequence<sizeof...(States)>;
    using Base = Fsm_impl<Ids, States...>;
public:
    constexpr Fsm() noexcept = default;

    template<typename... Ts>
    constexpr Fsm(Ts&&... ts) noexcept(std::is_nothrow_constructible_v<Base, Ts...>)
        : Base{std::forward<Ts>(ts)...}
        { }

    template<typename Event>
    void dispatch(Event&& event) noexcept
    {
        dispatch_event(*this, std::forward<Event>(event), Ids{});
    }
};
