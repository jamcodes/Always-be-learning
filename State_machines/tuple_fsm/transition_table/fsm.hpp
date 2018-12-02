#pragma once

#include "traits.hpp"
#include "transition_table.hpp"
#include <iostream>


/* transition_table */
/* --------------------------------------------------------------------------------------------- */
// template<typename State, typename Event>
// struct transition_table {
//     // using next_state = ...
//     // static constexpr inline auto guard = ...
//     // static constexpr inline auto action = ...
//     static_assert(always_false_v<State,Event>, "Unhandled state transition");
// };
/* --------------------------------------------------------------------------------------------- */


/* FsmState */
/* --------------------------------------------------------------------------------------------- */
template<size_type Idx, typename State>
class FsmState
{
private:
    State value_{};
public:
    using type = State;

    constexpr FsmState() noexcept = default;
    constexpr FsmState(FsmState&) = default;
    constexpr FsmState(FsmState const&) = default;
    constexpr FsmState(FsmState&&) noexcept = default;
    constexpr FsmState& operator=(FsmState const&) = default;
    constexpr FsmState& operator=(FsmState&&) noexcept = default;
    ~FsmState() noexcept = default;

    template<typename U
        /* , typename = std::enable_if_t<!std::is_same_v<FsmState<...>,std::decay_t<U>>> */>
    constexpr FsmState(U&& fvalue) noexcept(std::is_nothrow_constructible_v<State,U>)
        : value_{std::forward<U>(fvalue)} { }

    constexpr State&& Get() && noexcept { return std::move(value_); }
    constexpr State& Get() & noexcept { return value_; }
    constexpr State const& Get() const& noexcept { return value_; }
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

    constexpr inline size_type state_index() const noexcept { return state_; }
    constexpr inline void state_index(size_type new_state) noexcept { state_ = new_state; }

    template<typename FsmT, typename Event, size_type Idx, size_type... Idxs>
    friend constexpr inline void
    dispatch_event(FsmT&& fsm, Event&& event, Index_sequence<Idx,Idxs...>) noexcept;


    // // template<typename TTraits, bool HasGuard, bool HasNextState> friend struct transition;
    // template<typename FsmT, typename TTraits, typename State>
    // friend constexpr inline void transition(FsmT&& fsm, TTraits&& ttraits, State&& state) noexcept;
    // // template<typename TTraits, bool HasAction>                   friend struct fsm_action;
    // template<typename FsmT, typename TTraits>
    // friend constexpr inline void fsm_action(FsmT&& fsm, TTraits&& ttraits) noexcept;
    // // template<typename TTraits, typename FsmT, bool HasExit>      friend struct fsm_exit;
    // template<typename FsmT, typename State>
    // constexpr inline void fsm_exit(FsmT&& fsm, State&& state) noexcept;
    // // template<typename TTraits, typename FsmT, bool HasEntry>     friend struct fsm_entry;
    // template<typename FsmT, typename State>
    // constexpr inline void fsm_entry(FsmT&& fsm, State&& state) noexcept;

    template<size_type Idx, typename Idxs, typename... Sts>
    friend inline constexpr decltype(auto) Get(Fsm_impl<Idxs, Sts...>&) noexcept;
    template<size_type Idx, typename Idxs, typename... Sts>
    friend inline constexpr decltype(auto) Get(Fsm_impl<Idxs, Sts...> const&) noexcept;
    template<size_type Idx, typename Idxs, typename... Sts>
    friend inline constexpr decltype(auto) Get(Fsm_impl<Idxs, Sts...>&&) noexcept;
// private:
    size_type state_{};
};

template<typename FsmT, size_type Idx, size_type... Idxs>
constexpr inline size_type get_state_index(FsmT&& fsm, Index_sequence<Idx, Idxs...>) noexcept
{
    if (Idx == fsm.state_index()) {
        return Idx;
    } else if constexpr (sizeof...(Idxs) != 0) {
        get_state_index(std::forward<FsmT>(fsm), Index_sequence<Idxs...>{});
    }
}

template<typename FsmT> struct Get_index_sequence_impl { };
template<template<class,class...>class FsmT, typename Derived, typename... States>
struct Get_index_sequence_impl<FsmT<Derived, States...>> {
    using type = Make_index_sequence<sizeof...(States)>;
};
template<typename FsmT>
using Get_index_sequence = typename Get_index_sequence_impl<FsmT>::type;

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

    // // template<typename TTraits, bool HasGuard, bool HasNextState> friend struct transition;
    // template<typename FsmT, typename TTraits, typename State>
    // friend constexpr inline void transition(FsmT&& fsm, TTraits&& ttraits, State&& state) noexcept;
    // // template<typename TTraits, bool HasAction>                   friend struct fsm_action;
    // template<typename FsmT, typename TTraits>
    // friend constexpr inline void fsm_action(FsmT&& fsm, TTraits&& ttraits) noexcept;
    // // template<typename TTraits, typename FsmT, bool HasExit>      friend struct fsm_exit;
    // template<typename FsmT, typename State>
    // constexpr inline void fsm_exit(FsmT&& fsm, State&& state) noexcept;
    // // template<typename TTraits, typename FsmT, bool HasEntry>     friend struct fsm_entry;
    // template<typename FsmT, typename State>
    // constexpr inline void fsm_entry(FsmT&& fsm, State&& state) noexcept;

    // template<typename FsmT, typename State, typename Event,
    //          bool HasLogEvent>                                   friend struct fsm_log_event;

    // template<typename Event, typename FsmT, typename State>
    // friend constexpr inline std::enable_if_t<HasTraitsFor_v<FsmT, State, Event>>
    // state_transition(FsmT&& fsm, State&& state) noexcept;

    template<typename Event>
    constexpr inline void dispatch_event(Event&& event) noexcept
    {
        ::dispatch_event(*this, std::forward<Event>(event), Indices{});
    }

    constexpr decltype(auto) transition_table() const noexcept { return derived().transition_table(); }
    constexpr decltype(auto) transition_table() noexcept { return derived().transition_table(); }
    constexpr decltype(auto) self() & noexcept { return derived(); }
    constexpr decltype(auto) self() const& noexcept { return derived(); }
    constexpr decltype(auto) self() && noexcept { return derived(); }
// private:
    constexpr Derived& derived() & noexcept { return *static_cast<Derived*>(this); }
    constexpr Derived const& derived() const& noexcept { return *static_cast<Derived const*>(this); }
    constexpr Derived&& derived() && noexcept { return *static_cast<Derived*>(this); }
};


/* Get */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
template<size_type I, typename U>
inline constexpr FsmState_t<I,U>& Get_impl(FsmState<I,U>& st) noexcept
{
    return st.Get();
}
template<size_type I, typename U>
inline constexpr FsmState_t<I,U> const& Get_impl(FsmState<I,U> const& st) noexcept
{
    return st.Get();
}
template<size_type I, typename U>
inline constexpr FsmState_t<I,U>&& Get_impl(FsmState<I,U>&& st) noexcept
{
    return st.Get();
}

template<size_type I, typename Indices, typename... St>
inline constexpr decltype(auto) Get(Fsm_impl<Indices, St...>& fsm) noexcept
{
    return Get_impl<I>(fsm);
}
template<size_type I, typename Indices, typename... St>
inline constexpr decltype(auto) Get(Fsm_impl<Indices, St...> const& fsm) noexcept
{
    return Get_impl<I>(fsm);
}
template<size_type I, typename Indices, typename... St>
inline constexpr decltype(auto) Get(Fsm_impl<Indices, St...>&& fsm) noexcept
{
    return Get_impl<I>(fsm);
}

template<size_type Idx, typename FsmT>
using state_at = decltype( Get<Idx>(std::declval<FsmT>()) );
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* logging */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

// TODO: Decide if it's worth passing events down the call chain just to have additional logging
// information. As is, events are not passed down to `transition` from `dispatch event`.
// If it turns out useful for guards and actions to take an `event` parameter it might be worth it.
// Alternatively a dummy type<Event>{} could be passed to the logging functions, just to add the
// type information without providing access to the actual instance.

// Alternative implementation of logging templates - use struct partial specialization
// template<typename FsmT, typename State, typename Event,
//     bool = has_log_event_v<FsmT, State, Event>>
// struct fsm_log_event {
//     constexpr inline void operator()(FsmT const&, State const&, Event const&) const noexcept
//     {/* nop */}
// };

// template<typename FsmT, typename State, typename Event>
// struct fsm_log_event<FsmT, State, Event, true> {
//     constexpr inline void operator()(
//         FsmT const& fsm, State const& state, Event const& event) const noexcept {
//         fsm.logger().log_event(fsm, state, event);
//     }
// };

template<typename FsmT, typename State, typename Event>
std::enable_if_t<has_log_event_v<FsmT, State, Event>>
fsm_log_event(FsmT const& fsm, State const& state, Event const& event) noexcept
{
    FsmT::logger().log_event(fsm, state, event);
}

template<typename FsmT, typename State, typename Event>
std::enable_if_t<!has_log_event_v<FsmT, State, Event>>
fsm_log_event(FsmT const&, State const&, Event const&) noexcept {/* nop */}

template<typename FsmT, typename Guard/* , typename Event */>
std::enable_if_t<has_log_guard_v<FsmT, Guard/* , Event */>>
fsm_log_guard(FsmT const& fsm, Guard const& guard, /* Event const& event, */ bool result) noexcept
{
    FsmT::logger().log_guard(fsm, guard, /* event, */ result);
}

template<typename FsmT, typename Guard/* , typename Event */>
std::enable_if_t<!has_log_guard_v<FsmT, Guard/* , Event */>>
fsm_log_guard(FsmT const&, Guard const&, /* Event const&, */ bool) noexcept {/* nop */}

template<typename FsmT, typename Action/* , typename Event */>
std::enable_if_t<has_log_action_v<FsmT, Action/* , Event */>>
fsm_log_action(FsmT const& fsm, Action const& action/*,  Event const& event */) noexcept
{
    FsmT::logger().log_action(fsm, action/* , event */);
}

template<typename FsmT, typename Action/* , typename Event */>
std::enable_if_t<!has_log_action_v<FsmT, Action/* , Event */>>
fsm_log_action(FsmT const&, Action const&/* , Event const& */) noexcept {/* nop */}

template<typename FsmT, typename SrcState, typename DstState>
std::enable_if_t<has_log_state_change_v<FsmT, SrcState, DstState>>
fsm_log_state_change(FsmT const& fsm, SrcState const& src_state, DstState const& dst_state) noexcept
{
    FsmT::logger().log_state_change(fsm, src_state, dst_state);
}

template<typename FsmT, typename SrcState, typename DstState>
std::enable_if_t<!has_log_state_change_v<FsmT, SrcState, DstState>>
fsm_log_state_change(FsmT const&, SrcState const&, DstState const&) noexcept {/* nop */}

template<typename FsmT, typename State>
std::enable_if_t<has_log_exit_v<FsmT, State>>
fsm_log_exit(FsmT const& fsm, State const& state) noexcept
{
    FsmT::logger().log_exit(fsm, state);
}

template<typename FsmT, typename State>
std::enable_if_t<!has_log_exit_v<FsmT, State>>
fsm_log_exit(FsmT const&, State const&) noexcept {/* nop */}

template<typename FsmT, typename State>
std::enable_if_t<has_log_entry_v<FsmT, State>>
fsm_log_entry(FsmT const& fsm, State const& state) noexcept
{
    FsmT::logger().log_entry(fsm, state);
}

template<typename FsmT, typename State>
std::enable_if_t<!has_log_entry_v<FsmT, State>>
fsm_log_entry(FsmT const&, State const&) noexcept {/* nop */}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* dispatch */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// template<typename TTraits, bool = has_action_v<TTraits>>
// struct fsm_action {
//     template<typename FsmT>
//     constexpr inline void operator()(FsmT&&) const noexcept {/* nop */}
// };

// template<typename TTraits>
// struct fsm_action<TTraits, true> {
//     template<typename FsmT>
//     constexpr inline void operator()(FsmT&& fsm) const noexcept {
//         static_assert(is_valid_action_v<decltype(TTraits::action), decltype(fsm.derived())>);
//         fsm_log_action(fsm.derived(), TTraits::action);
//         TTraits::action(std::forward<FsmT>(fsm).derived());
//     }
// };

template<typename FsmT, typename TTraits>
constexpr inline std::enable_if_t<has_action_v<TTraits, Self<FsmT>>>
fsm_action(FsmT&& fsm, TTraits&& ttraits) noexcept {
    static_assert(is_valid_action_v<decltype(ttraits.action), decltype(fsm.derived())>);
    // std::cerr << "+ Action\n";
    fsm_log_action(fsm.derived(), ttraits.action);
    ttraits.action(std::forward<FsmT>(fsm).derived());
}
template<typename FsmT, typename TTraits>
constexpr inline std::enable_if_t<!has_action_v<TTraits, Self<FsmT>>>
fsm_action(FsmT&&, TTraits&&) noexcept {
    /* nop */
    // std::cerr << "- Action\n";
}

// template<typename TTraits, typename FsmT,
//     bool = has_exit_v<TTraits, decltype(std::declval<FsmT>().derived())>>
// struct fsm_exit {
//     template<typename FsmT_, typename State>
//     constexpr inline void operator()(FsmT_&&, State&&) const noexcept {/* nop */}
// };

// template<typename TTraits, typename FsmT>
// struct fsm_exit<TTraits, FsmT, true> {
//     template<typename FsmT_, typename State>
//     constexpr inline void operator()(FsmT_&& fsm, State&& state) const noexcept
//     {
//         fsm_log_exit(fsm.derived(), state);
//         std::forward<State>(state).exit(std::forward<FsmT_>(fsm).derived());
//     }
// };

template<typename FsmT, typename State>
constexpr inline std::enable_if_t<has_exit_v<State, Self<FsmT>>>
fsm_exit(FsmT&& fsm, State&& state) noexcept {
    fsm_log_exit(fsm.derived(), state);
    std::forward<State>(state).exit(std::forward<FsmT>(fsm).derived());
}

template<typename FsmT, typename State>
constexpr inline std::enable_if_t<!has_exit_v<State, Self<FsmT>>>
fsm_exit(FsmT&&, State&&) noexcept {/* nop */}

// template<typename TTraits, typename FsmT,
//     bool = has_entry_v<TTraits, decltype(std::declval<FsmT>().derived())>>
// struct fsm_entry {
//     template<typename FsmT_, typename State>
//     constexpr inline void operator()(FsmT_&&, State&&) const noexcept {/* nop */}
// };

// template<typename TTraits, typename FsmT>
// struct fsm_entry<TTraits, FsmT, true> {
//     template<typename FsmT_, typename State>
//     constexpr inline void operator()(FsmT_&& fsm, State&& state) const noexcept
//     {
//         fsm_log_entry(fsm.derived(), state);
//         std::forward<State>(state).entry(std::forward<FsmT_>(fsm).derived());
//     }
// };

template<typename FsmT, typename State>
constexpr inline std::enable_if_t<!has_entry_v<State, Self<FsmT>>>
fsm_entry(FsmT&&, State&&) noexcept {/* nop */}

template<typename FsmT, typename State>
constexpr inline std::enable_if_t<has_entry_v<State, Self<FsmT>>>
fsm_entry(FsmT&& fsm, State&& state) noexcept {
    fsm_log_entry(fsm.derived(), state);
    std::forward<State>(state).entry(std::forward<FsmT>(fsm).derived());
}

// template<typename TTraits, bool = has_guard_v<TTraits>, bool = has_next_state_v<TTraits>>
// struct transition {
//     template<typename FsmT, typename State>
//     constexpr inline void operator()(FsmT&& fsm, State&&) const noexcept {
//         // std::cerr << "-Guard, -NextState\n";
//         fsm_action<TTraits>{}(fsm);
//     }
// };

// -Guard, -NextState
template<typename FsmT, typename TTraits, typename State>
constexpr inline std::enable_if_t<!has_guard_v<TTraits, Self<FsmT>> && !has_next_state_v<TTraits>>
transition(FsmT&& fsm, TTraits&& ttraits, State&&) noexcept {
    // std::cerr << "-Guard, -NextState\n";
    fsm_action(std::forward<FsmT>(fsm), std::forward<TTraits>(ttraits));
}

// +Guard, -NextState
// template<typename TTraits>
// struct transition<TTraits, true, false> {
//     template<typename FsmT, typename State>
//     constexpr inline void operator()(FsmT&& fsm, State&&) const noexcept {
//         // std::cerr << "+Guard, -NextState\n";
//         static_assert(is_valid_guard_v<decltype(TTraits::guard),
//                       decltype(std::declval<FsmT>().derived())>);
//         const auto guard_result = TTraits::guard(fsm.derived());
//         fsm_log_guard(fsm.derived(), TTraits::guard, guard_result);
//         if (guard_result) {
//             fsm_action<TTraits>{}(fsm);
//         }
//     }
// };

// +Guard, -NextState
template<typename FsmT, typename TTraits, typename State>
constexpr inline std::enable_if_t<has_guard_v<TTraits,Self<FsmT>> && !has_next_state_v<TTraits>>
transition(FsmT&& fsm, TTraits&& ttraits, State&& state) noexcept {
    // std::cerr << "+Guard, -NextState\n";
    static_assert(is_valid_guard_v<decltype(ttraits.guard), decltype(fsm.derived())>);
    const auto guard_result = ttraits.guard(fsm.derived());
    fsm_log_guard(fsm.derived(), ttraits.guard, guard_result);
    if (guard_result) {
        fsm_action(std::forward<FsmT>(fsm), std::forward<TTraits>(ttraits));
    }
}

// -Guard, +NextState
// template<typename TTraits>
// struct transition<TTraits, false, true> {
//     template<typename FsmT, typename State>
//     constexpr inline void operator()(FsmT&& fsm, State&& state) const noexcept {
//         // std::cerr << "-Guard, +NextState\n";
//         fsm_exit<TTraits, FsmT>{}(fsm, state);
//         using fsm_statelist = get_state_list_t<std::decay_t<FsmT>>;
//         fsm_action<TTraits>{}(fsm);
//         constexpr auto next_state_idx = state_index_v<fsm_statelist, Next_state<TTraits>>;
//         fsm.state_ = next_state_idx;
//         auto&& next_state = Get<next_state_idx>(fsm);
//         fsm_log_state_change(fsm.derived(), state, next_state);
//         fsm_entry<TTraits, FsmT>{}(fsm, std::forward<decltype(next_state)>(next_state));
//     }
// };

// -Guard, +NextState
template<typename FsmT, typename TTraits, typename State>
constexpr inline std::enable_if_t<!has_guard_v<TTraits,Self<FsmT>> && has_next_state_v<TTraits>>
transition(FsmT&& fsm, TTraits&& ttraits, State&& state) noexcept {
    // std::cerr << "-Guard, +NextState\n";
    fsm_exit(fsm, state);
    fsm_action(fsm, ttraits);
    using fsm_statelist = get_state_list_t<std::decay_t<FsmT>>;
    constexpr auto next_state_idx = state_index_v<fsm_statelist, Next_state<std::decay_t<TTraits>>>;
    fsm.state_ = next_state_idx;
    auto&& next_state = Get<next_state_idx>(fsm);
    fsm_log_state_change(fsm.derived(), state, next_state);
    fsm_entry(std::forward<FsmT>(fsm), std::forward<decltype(next_state)>(next_state));
}

// +Guard, +NextState
// template<typename TTraits>
// struct transition<TTraits, true, true> {
//     template<typename FsmT, typename State>
//     constexpr inline void operator()(FsmT&& fsm, State&& state) const noexcept {
//         // std::cerr << "+Guard, +NextState\n";
//         static_assert(is_valid_guard_v<decltype(TTraits::guard),
//                       decltype(std::declval<FsmT>().derived())>);
//         using fsm_statelist = get_state_list_t<std::decay_t<FsmT>>;
//         const auto guard_result = TTraits::guard(fsm.derived());
//         fsm_log_guard(fsm.derived(), TTraits::guard, guard_result);
//         if (guard_result) {
//             fsm_exit<TTraits, FsmT>{}(fsm, state);
//             fsm_action<TTraits>{}(fsm);
//             constexpr auto next_state_idx = state_index_v<fsm_statelist, Next_state<TTraits>>;
//             fsm.state_ = next_state_idx;
//             auto&& next_state = Get<next_state_idx>(fsm);
//             fsm_log_state_change(fsm.derived(), state, next_state);
//             fsm_entry<TTraits, FsmT>{}(fsm, std::forward<decltype(next_state)>(next_state));
//         }
//     }
// };

// +Guard, +NextState
template<typename FsmT, typename TTraits, typename State>
constexpr inline std::enable_if_t<has_guard_v<TTraits,Self<FsmT>> && has_next_state_v<TTraits>>
transition(FsmT&& fsm, TTraits&& ttraits, State&& state) noexcept {
    // std::cerr << "+Guard, +NextState\n";
    static_assert(is_valid_guard_v<decltype(ttraits.guard), decltype(fsm.self())>);
    const auto guard_result = ttraits.guard(fsm.self());
    fsm_log_guard(fsm.self(), ttraits.guard, guard_result);
    if (guard_result) {
        fsm_exit(fsm, state);
        fsm_action(fsm, ttraits);
        using fsm_statelist = get_state_list_t<std::decay_t<FsmT>>;
        constexpr auto next_state_idx = state_index_v<fsm_statelist, Next_state<std::decay_t<TTraits>>>;
        fsm.state_ = next_state_idx;
        auto&& next_state = Get<next_state_idx>(fsm);
        fsm_log_state_change(fsm.self(), state, next_state);
        fsm_entry(std::forward<FsmT>(fsm), std::forward<decltype(next_state)>(next_state));
    }
}

template<typename Event, typename FsmT, typename State>
constexpr inline std::enable_if_t<!HasTraitsFor_v<FsmT, State, Event>>
state_transition(FsmT&&, State&&) noexcept {
    /* nop */
    // std::cerr << "- " << __PRETTY_FUNCTION__ << "\n";
}

template<typename Event, typename FsmT, typename State>
constexpr inline std::enable_if_t<HasTraitsFor_v<FsmT, State, Event>>
state_transition(FsmT&& fsm, State&& state) noexcept {
    using state_t = std::decay_t<State>;
    using event_t = std::decay_t<Event>;
    // std::cerr << "+ " << __PRETTY_FUNCTION__ << "\n";
    // using transition_traits = transition_table<state_t, event_t>;
    // using transition_traits = decltype(Get_transition_traits<state_t, event_t>(
    //                                    fsm.transition_table()));
    auto&& ttraits = Get_transition_traits<state_t, event_t>(fsm.transition_table());
    transition(std::forward<FsmT>(fsm),
        std::forward<decltype(ttraits)>(ttraits), std::forward<State>(state));
}

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
        // using transition_traits = transition_table<state_t, event_t>;
        // using transition_traits = Get_transition_traits<FsmT, state_t, event_t>;
        auto&& state = Get<Idx>(std::forward<FsmT>(fsm));
        // using derived_t = std::decay_t<decltype(fsm.derived())>;
        // fsm_log_event<derived_t, state_t, Event>{}(fsm.derived(), state, event);
        fsm_log_event(fsm.derived(), state, event);
        if constexpr (has_handle_event_v<state_t, decltype(fsm.derived()), Event>) {
            state.handle_event(fsm.derived(), std::forward<Event>(event));
        }
        // transition<transition_traits>{}(
        //     std::forward<FsmT>(fsm),std::forward<decltype(state)>(state));
        state_transition<event_t>(std::forward<FsmT>(fsm), std::forward<decltype(state)>(state));
    }
    else if constexpr (sizeof...(Idxs) != 0) {
        dispatch_event(
            std::forward<FsmT>(fsm), std::forward<Event>(event), Index_sequence<Idxs...>{});
    }
}


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* --------------------------------------------------------------------------------------------- */
