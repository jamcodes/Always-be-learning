#pragma once

#include "traits.hpp"
#include <iostream>


/* transition_table */
/* --------------------------------------------------------------------------------------------- */
template<typename State, typename Event>
struct transition_table {
    // using next_state = ...
    // static constexpr inline auto guard = ...
    // static constexpr inline auto action = ...
    static_assert(always_false_v<State,Event>, "Unhandled state transition");
};
/* --------------------------------------------------------------------------------------------- */


/* FsmState */
/* --------------------------------------------------------------------------------------------- */
template<size_type Idx, typename State>
class FsmState
{
private:
    // State value_{};
public:
    using type = State;

    constexpr FsmState() noexcept = default;
    constexpr FsmState(FsmState&) = default;
    constexpr FsmState(FsmState const&) = default;
    constexpr FsmState(FsmState&&) noexcept = default;
    constexpr FsmState& operator=(FsmState const&) = default;
    constexpr FsmState& operator=(FsmState&&) noexcept = default;
    ~FsmState() noexcept = default;

    // template<typename U
    //     /* , typename = std::enable_if_t<!std::is_same_v<FsmState<...>,std::decay_t<U>>> */>
    // constexpr FsmState(U&& fvalue) noexcept(std::is_nothrow_constructible_v<State,U>)
    //     : value_{std::forward<U>(fvalue)} { }

    // constexpr State&& Get() && noexcept { return std::move(value_); }
    // constexpr State& Get() & noexcept { return value_; }
    // constexpr State const& Get() const& noexcept { return value_; }
};

template<size_type Idx, typename State>
using FsmState_t = typename FsmState<Idx,State>::type;

template<size_type Idx, typename State>
inline constexpr FsmState_t<Idx,State>& Get_impl(FsmState<Idx,State>& state) noexcept;
template<size_type Idx, typename State>
inline constexpr FsmState_t<Idx,State> const& Get_impl(FsmState<Idx,State> const& state) noexcept;
template<size_type Idx, typename State>
inline constexpr FsmState_t<Idx,State>&& Get_impl(FsmState<Idx,State>&& state) noexcept;
/* --------------------------------------------------------------------------------------------- */


/* Fsm */
/* --------------------------------------------------------------------------------------------- */
template<typename IndexSequence, typename... States>
class Fsm_impl;

template<size_type... Indices, typename... States>
class Fsm_impl<Index_sequence<Indices...>, States...> : private FsmState<Indices, States>...
{
public:
    constexpr Fsm_impl() noexcept = default;

    template<typename State>
    constexpr Fsm_impl(initial_state<State>) noexcept
        : state_{state_index_v<typelist<States...>,State>} { }

    template<typename... Ts>
    constexpr Fsm_impl(Ts&&... states) noexcept((... && std::is_nothrow_constructible_v<States, Ts>))
        : FsmState<Indices,States>{std::forward<Ts>(states)}...
        { }

    template<typename T>
    constexpr void set_initial_state(initial_state<T>) noexcept
    {
        state_ = state_index_v<typelist<States...>,T>;
    }

    template<typename FsmT, typename Event, size_type Idx, size_type... Idxs>
    friend constexpr inline void
    dispatch_event(FsmT&& fsm, Event&& event, Index_sequence<Idx,Idxs...>) noexcept;

    template<typename TTraits, bool HasGuard, bool HasNextState> friend struct transition;
    template<typename TTraits, bool HasAction>                   friend struct fsm_action;
    template<typename TTraits, typename FsmT, bool HasExit>      friend struct fsm_exit;
    template<typename TTraits, typename FsmT, bool HasEntry>     friend struct fsm_entry;

    // template<size_type Idx, typename Idxs, typename... Sts>
    // friend inline constexpr FsmState_t<Idx,State>& Get(Fsm_impl<Idxs, Sts...>&) noexcept;
    // template<size_type Idx, typename Idxs, typename... Sts>
    // friend inline constexpr FsmState_t<Idx,State> const& Get(Fsm_impl<Idxs, Sts...> const&) noexcept;
    // template<size_type Idx, typename Idxs, typename... Sts>
    // friend inline constexpr FsmState_t<Idx,State>&& Get(Fsm_impl<Idxs, Sts...>&&) noexcept;
private:
    size_type state_{};
};

template<typename FsmT, typename Event, size_type Idx, size_type... Idxs>
constexpr inline void
dispatch_event(FsmT&& fsm, Event&& event, Index_sequence<Idx,Idxs...>) noexcept;

template<typename Derived, typename... States>
class Fsm : public Fsm_impl<Make_index_sequence<sizeof...(States)>, States...>
{
    using Indices = Make_index_sequence<sizeof...(States)>;
    using Base = Fsm_impl<Indices, States...>;
public:
    constexpr Fsm() noexcept = default;

    using Base::Base;

    using Base::set_initial_state;

    template<typename FsmT, typename Event, size_type Idx, size_type... Idxs>
    friend constexpr inline void
    dispatch_event(FsmT&& fsm, Event&& event, Index_sequence<Idx,Idxs...>) noexcept;

    template<typename TTraits, bool HasGuard, bool HasNextState> friend struct transition;
    template<typename TTraits, bool HasAction>                   friend struct fsm_action;
    template<typename TTraits, typename FsmT, bool HasExit>      friend struct fsm_exit;
    template<typename TTraits, typename FsmT, bool HasEntry>     friend struct fsm_entry;

    template<typename Event>
    constexpr inline void dispatch_event(Event&& event) noexcept
    {
        ::dispatch_event(*this, std::forward<Event>(event), Indices{});
    }

private:
    Derived& derived() & noexcept { return *static_cast<Derived*>(this); }
    Derived const& derived() const& noexcept { return *static_cast<Derived const*>(this); }
    Derived&& derived() && noexcept { return *static_cast<Derived*>(this); }
};


/* Get */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// template<size_type I, typename U>
// inline constexpr FsmState_t<I,U>& Get_impl(FsmState<I,U>& st) noexcept
// {
//     return st.Get();
// }
// template<size_type I, typename U>
// inline constexpr FsmState_t<I,U> const& Get_impl(FsmState<I,U> const& st) noexcept
// {
//     return st.Get();
// }
// template<size_type I, typename U>
// inline constexpr FsmState_t<I,U>&& Get_impl(FsmState<I,U>&& st) noexcept
// {
//     return st.Get();
// }

// template<size_type I, typename Indices, typename... St>
// inline constexpr decltype(auto) Get(Fsm_impl<Indices, St...>& fsm) noexcept
// {
//     return Get_impl<I>(fsm);
// }
// template<size_type I, typename Indices, typename... St>
// inline constexpr decltype(auto) Get(Fsm_impl<Indices, St...> const& fsm) noexcept
// {
//     return Get_impl<I>(fsm);
// }
// template<size_type I, typename Indices, typename... St>
// inline constexpr decltype(auto) Get(Fsm_impl<Indices, St...>&& fsm) noexcept
// {
//     return Get_impl<I>(fsm);
// }

template<size_type Idx, typename FsmT>
using state_at = decltype( Get_impl<Idx>(std::declval<FsmT>()) );
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* dispatch */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
template<typename TTraits, bool = has_action_v<TTraits>>
struct fsm_action {
    template<typename FsmT>
    constexpr inline void operator()(FsmT&&) const noexcept {/* nop */}
};

template<typename TTraits>
struct fsm_action<TTraits, true> {
    template<typename FsmT>
    constexpr inline void operator()(FsmT&& fsm) const noexcept {
        static_assert(is_valid_action_v<decltype(TTraits::action), decltype(fsm.derived())>);
        TTraits::action(std::forward<FsmT>(fsm).derived());
    }
};

template<typename TTraits, typename FsmT,
    bool = has_exit_v<TTraits, decltype(std::declval<FsmT>().derived())>>
struct fsm_exit {
    template<typename FsmT_>
    constexpr inline void operator()(FsmT_&&) const noexcept {/* nop */}
};

template<template<class,class>class TTraits, typename State, typename Event, typename FsmT>
struct fsm_exit<TTraits<State,Event>, FsmT, true> {
    template<typename FsmT_>
    constexpr inline void operator()(FsmT_&& fsm) const noexcept
    {
        State::exit(std::forward<FsmT_>(fsm).derived());
    }
};

template<typename TTraits, typename FsmT,
    bool = has_entry_v<TTraits, decltype(std::declval<FsmT>().derived())>>
struct fsm_entry {
    template<typename FsmT_>
    constexpr inline void operator()(FsmT_&&) const noexcept {/* nop */}
};

template<template<class,class>class TTraits, typename FsmT, typename State, typename Event>
struct fsm_entry<TTraits<State,Event>, FsmT, true> {
    template<typename FsmT_>
    constexpr inline void operator()(FsmT_&& fsm) const noexcept
    {
        State::entry(std::forward<FsmT_>(fsm).derived());
    }
};

template<typename TTraits, bool = has_guard_v<TTraits>, bool = has_next_state_v<TTraits>>
struct transition {
    template<typename FsmT>
    constexpr inline void operator()(FsmT&& fsm) const noexcept {
        // std::cerr << "-Guard, -NextState\n";
        fsm_action<TTraits>{}(fsm);
    }
};

// +Guard, -NextState
template<template<class,class>class TTraits, typename State, typename Event>
struct transition<TTraits<State,Event>, true, false> {
    using traits_t = TTraits<State,Event>;
    template<typename FsmT>
    constexpr inline void operator()(FsmT&& fsm) const noexcept {
        // std::cerr << "+Guard, -NextState\n";
        static_assert(is_valid_guard_v<decltype(traits_t::guard),
            decltype(fsm.derived())>);
        if (traits_t::guard(fsm.derived())) {
            fsm_action<traits_t>{}(fsm);
        }
    }
};

// -Guard, +NextState
template<template<class,class>class TTraits, typename State, typename Event>
struct transition<TTraits<State,Event>, false, true> {
    using traits_t = TTraits<State,Event>;
    template<typename FsmT>
    constexpr inline void operator()(FsmT&& fsm) const noexcept {
        // std::cerr << "-Guard, +NextState\n";
        fsm_exit<traits_t, FsmT>{}(fsm);
        using fsm_statelist = get_state_list_t<std::decay_t<FsmT>>;
        fsm_action<traits_t>{}(fsm);
        constexpr auto next_state_idx = state_index_v<fsm_statelist, Next_state<traits_t>>;
        fsm.state_ = next_state_idx;
        fsm_entry<TTraits<Next_state<traits_t>,Event>, FsmT>{}(fsm);
    }
};

// +Guard, +NextState
template<template<class,class>class TTraits, typename State, typename Event>
struct transition<TTraits<State,Event>, true, true> {
    using traits_t = TTraits<State,Event>;
    template<typename FsmT>
    constexpr inline void operator()(FsmT&& fsm) const noexcept {
        // std::cerr << "+Guard, +NextState\n";
        static_assert(is_valid_guard_v<decltype(traits_t::guard), decltype(fsm.derived())>);
        using fsm_statelist = get_state_list_t<std::decay_t<FsmT>>;
        if (traits_t::guard(fsm.derived())) {
            fsm_exit<traits_t, FsmT>{}(fsm);
            fsm_action<traits_t>{}(fsm);
            constexpr auto next_state_idx = state_index_v<fsm_statelist, Next_state<traits_t>>;
            fsm.state_ = next_state_idx;
            fsm_entry<TTraits<Next_state<traits_t>,Event>, FsmT>{}(fsm);
        }
    }
};

// 1. Evaluate the guard condition associated with the transition and perform the following steps
//    only if the guard evaluates to TRUE.
// 2. Exit the source state configuration.
// 3. Execute the actions associated with the transition.
// 4. Enter the target state configuration.
// dispatch_event using `transition` for branching during event handling
template<typename FsmT, typename Event, size_type Idx, size_type... Idxs>
constexpr inline void
dispatch_event(FsmT&& fsm, Event&& event, Index_sequence<Idx,Idxs...>) noexcept
{
    if (Idx == fsm.state_) {
        using state_t = std::decay_t<state_at<Idx, FsmT>>;
        using event_t = std::decay_t<Event>;
        using transition_traits = transition_table<state_t, event_t>;
        // std::cerr << "state index = " << Idx << "\n";
        // auto&& state = Get<Idx>(std::forward<FsmT>(fsm));
        if constexpr (has_handle_event_v<state_t, decltype(fsm.derived()), Event>) {
            // state.handle_event(fsm.derived(), std::forward<Event>(event));
            state_t::handle_event(fsm.derived(), std::forward<Event>(event));
        }
        transition<transition_traits>{}(std::forward<FsmT>(fsm));
    }
    else if constexpr (sizeof...(Idxs) != 0) {
        dispatch_event(
            std::forward<FsmT>(fsm), std::forward<Event>(event), Index_sequence<Idxs...>{});
    }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* --------------------------------------------------------------------------------------------- */
