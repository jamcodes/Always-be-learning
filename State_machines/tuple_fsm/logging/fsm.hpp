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

    template<typename FsmT, typename Event, size_type Idx, size_type... Idxs>
    friend constexpr inline void
    dispatch_event(FsmT&& fsm, Event&& event, Index_sequence<Idx,Idxs...>) noexcept;

    template<typename TTraits, bool HasGuard, bool HasNextState> friend struct transition;
    template<typename TTraits, bool HasAction>                   friend struct fsm_action;
    template<typename TTraits, typename FsmT, bool HasExit>      friend struct fsm_exit;
    template<typename TTraits, typename FsmT, bool HasEntry>     friend struct fsm_entry;

    template<size_type Idx, typename Idxs, typename... Sts>
    friend inline constexpr decltype(auto) Get(Fsm_impl<Idxs, Sts...>&) noexcept;
    template<size_type Idx, typename Idxs, typename... Sts>
    friend inline constexpr decltype(auto) Get(Fsm_impl<Idxs, Sts...> const&) noexcept;
    template<size_type Idx, typename Idxs, typename... Sts>
    friend inline constexpr decltype(auto) Get(Fsm_impl<Idxs, Sts...>&&) noexcept;
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
    // template<typename FsmT, typename State, typename Event,
    //          bool HasLogEvent>                                   friend struct fsm_log_event;

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
    template<typename FsmT_, typename State>
    constexpr inline void operator()(FsmT_&&, State&&) const noexcept {/* nop */}
};

template<typename TTraits, typename FsmT>
struct fsm_exit<TTraits, FsmT, true> {
    template<typename FsmT_, typename State>
    constexpr inline void operator()(FsmT_&& fsm, State&& state) const noexcept
    {
        std::forward<State>(state).exit(std::forward<FsmT_>(fsm).derived());
    }
};

template<typename TTraits, typename FsmT,
    bool = has_entry_v<TTraits, decltype(std::declval<FsmT>().derived())>>
struct fsm_entry {
    template<typename FsmT_, typename State>
    constexpr inline void operator()(FsmT_&&, State&&) const noexcept {/* nop */}
};

template<typename TTraits, typename FsmT>
struct fsm_entry<TTraits, FsmT, true> {
    template<typename FsmT_, typename State>
    constexpr inline void operator()(FsmT_&& fsm, State&& state) const noexcept
    {
        std::forward<State>(state).entry(std::forward<FsmT_>(fsm).derived());
    }
};

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

template<typename FsmT, typename Guard, typename Event>
std::enable_if_t<has_log_guard_v<FsmT, Guard, Event>>
fsm_log_guard(FsmT const& fsm, Guard const& guard, Event const& event, bool result) noexcept
{
    FsmT::logger().log_guard(fsm, guard, event, result);
}

template<typename FsmT, typename Guard, typename Event>
std::enable_if_t<!has_log_guard_v<FsmT, Guard, Event>>
fsm_log_guard(FsmT const&, Guard const&, Event const&, bool) noexcept {/* nop */}

template<typename FsmT, typename Action, typename Event>
std::enable_if_t<has_log_action_v<FsmT, Action, Event>>
fsm_log_action(FsmT const& fsm, Action const& action, Event const& event) noexcept
{
    FsmT::logger().log_action(fsm, action, event);
}

template<typename FsmT, typename Action, typename Event>
std::enable_if_t<!has_log_action_v<FsmT, Action, Event>>
fsm_log_action(FsmT const&, Action const&, Event const&) noexcept {/* nop */}

template<typename FsmT, typename SrcState, typename DstState>
std::enable_if_t<has_log_state_change_v<FsmT, SrcState, DstState>>
fsm_log_state_change(FsmT const& fsm, SrcState const& src_state, DstState const& dst_state) noexcept
{
    FsmT::logger().log_state_change(fsm, src_state, dst_state);
}

template<typename FsmT, typename SrcState, typename DstState>
std::enable_if_t<!has_log_state_change_v<FsmT, SrcState, DstState>>
fsm_log_state_change(FsmT const& fsm, SrcState const& src_state, DstState const& dst_state) noexcept
{/* nop */}

template<typename TTraits, bool = has_guard_v<TTraits>, bool = has_next_state_v<TTraits>>
struct transition {
    template<typename FsmT, typename State>
    constexpr inline void operator()(FsmT&& fsm, State&&) const noexcept {
        // std::cerr << "-Guard, -NextState\n";
        fsm_action<TTraits>{}(fsm);
    }
};

// +Guard, -NextState
template<typename TTraits>
struct transition<TTraits, true, false> {
    template<typename FsmT, typename State>
    constexpr inline void operator()(FsmT&& fsm, State&&) const noexcept {
        // std::cerr << "+Guard, -NextState\n";
        static_assert(is_valid_guard_v<decltype(TTraits::guard),
                      decltype(std::declval<FsmT>().derived())>);
        if (TTraits::guard(fsm.derived())) {
            fsm_action<TTraits>{}(fsm);
        }
    }
};

// -Guard, +NextState
template<typename TTraits>
struct transition<TTraits, false, true> {
    template<typename FsmT, typename State>
    constexpr inline void operator()(FsmT&& fsm, State&& state) const noexcept {
        // std::cerr << "-Guard, +NextState\n";
        fsm_exit<TTraits, FsmT>{}(fsm, state);
        using fsm_statelist = get_state_list_t<std::decay_t<FsmT>>;
        fsm_action<TTraits>{}(fsm);
        constexpr auto next_state_idx = state_index_v<fsm_statelist, Next_state<TTraits>>;
        fsm.state_ = next_state_idx;
        fsm_entry<TTraits, FsmT>{}(fsm, Get<next_state_idx>(fsm));
    }
};

// +Guard, +NextState
template<typename TTraits>
struct transition<TTraits, true, true> {
    template<typename FsmT, typename State>
    constexpr inline void operator()(FsmT&& fsm, State&& state) const noexcept {
        // std::cerr << "+Guard, +NextState\n";
        static_assert(is_valid_guard_v<decltype(TTraits::guard),
                      decltype(std::declval<FsmT>().derived())>);
        using fsm_statelist = get_state_list_t<std::decay_t<FsmT>>;
        if (TTraits::guard(fsm.derived())) {
            fsm_exit<TTraits, FsmT>{}(fsm, state);
            fsm_action<TTraits>{}(fsm);
            constexpr auto next_state_idx = state_index_v<fsm_statelist, Next_state<TTraits>>;
            fsm.state_ = next_state_idx;
            fsm_entry<TTraits, FsmT>{}(fsm, Get<next_state_idx>(fsm));
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
        using derived_t = std::decay_t<decltype(fsm.derived())>;
        using transition_traits = transition_table<state_t, event_t>;
        // std::cerr << "state index = " << Idx << "\n";
        auto&& state = Get<Idx>(std::forward<FsmT>(fsm));
        // fsm_log_event<derived_t, state_t, Event>{}(fsm.derived(), state, event);
        fsm_log_event(fsm.derived(), state, event);
        if constexpr (has_handle_event_v<state_t, decltype(fsm.derived()), Event>) {
            // std::cerr << "has_handle_event == true\n";
            state.handle_event(fsm.derived(), std::forward<Event>(event));
        }
        transition<transition_traits>{}(
            std::forward<FsmT>(fsm),std::forward<decltype(state)>(state));
    }
    else if constexpr (sizeof...(Idxs) != 0) {
        dispatch_event(
            std::forward<FsmT>(fsm), std::forward<Event>(event), Index_sequence<Idxs...>{});
    }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* --------------------------------------------------------------------------------------------- */
