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

    template<typename... Ts>
    constexpr Fsm_impl(Ts&&... states) noexcept((... && std::is_nothrow_constructible_v<States, Ts>))
        : FsmState<Indices,States>{std::forward<Ts>(states)}...
        { }

    template<typename FsmT, typename Event, size_type Idx, size_type... Idxs>
    friend constexpr inline void
    dispatch_event(FsmT&& fsm, Event&& event, Index_sequence<Idx,Idxs...>) noexcept;

    template<typename TTraits, bool HasGuard, bool HasAction, bool HasNextState,
             bool HasExit, bool HasEntry>
    friend struct transition;
    // template<size_type Idx, typename Event>
    // constexpr inline void transition(Event&& event) noexcept;

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

template<typename... States>
class Fsm : public Fsm_impl<Make_index_sequence<sizeof...(States)>, States...>
{
    using Indices = Make_index_sequence<sizeof...(States)>;
    using Base = Fsm_impl<Indices, States...>;
public:
    using State_list = typelist<States...>;
    constexpr Fsm() noexcept = default;

    using Base::Base;

    template<typename FsmT, typename Event, size_type Idx, size_type... Idxs>
    friend constexpr inline void
    dispatch_event(FsmT&& fsm, Event&& event, Index_sequence<Idx,Idxs...>) noexcept;

    template<typename TTraits, bool HasGuard, bool HasAction, bool HasNextState,
             bool HasExit, bool HasEntry>
    friend struct transition;

    template<typename Event>
    constexpr inline void dispatch_event(Event&& event) noexcept
    {
        ::dispatch_event(*this, std::forward<Event>(event), Indices{});
    }
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
template<typename TTraits, /* typename FsmT, typename State, */
        bool = has_guard_v<TTraits>, bool = has_action_v<TTraits>, bool = has_next_state_v<TTraits>,
        bool = has_exit_v<TTraits>, bool = has_entry_v<TTraits>>
struct transition {
    // default - all false -> nop
    template<typename FsmT, typename State>
    constexpr inline void operator()(FsmT&&, State&&) const noexcept {
        std::cerr << "-Guard, -Action, -NextState, -Exit, -Entry\n";
    }
};

// let this case fall back to the default for now
// template<typename TTraits, /* typename FsmT, typename State, */ bool HasExit, bool HasEntry>
// struct transition<TTraits, /* FsmT, State, */ false, false, false, HasExit, HasEntry> {
//     // has a guard, but no action, no next state -> nop
//     // warn against this with a static_assert?
// };

// Guard, Action, No next_state
// This is not compliant with UML -> execute Action if Guard is true,
// but Action will be executed without calling the exit action and no state change will occur
template<typename TTraits, /* typename FsmT, typename State, */ bool HasExit, bool HasEntry>
struct transition<TTraits, /* FsmT, State, */ true, true, false, HasExit, HasEntry> {
    template<typename FsmT_, typename State_>
    constexpr inline void operator()(FsmT_&& fsm, State_&& state) const noexcept
    {
        static_assert(is_valid_guard_v<decltype(TTraits::guard)>);
        static_assert(is_valid_action_v<decltype(TTraits::action)>);
        std::cerr << "+Guard, +Action, -NextState, ?Exit, ?Entry\n";
        if (TTraits::guard()) {
            TTraits::action();
        }
    }
};

// Guard, No Action, next_state, No Exit, No Entry
template<typename TTraits>
struct transition<TTraits, true, false, true, false, false> {
    template<typename FsmT, typename State>
    constexpr inline void operator()(FsmT&& fsm, State&&) const noexcept
    {
        static_assert(is_valid_guard_v<decltype(TTraits::guard)>);
        std::cerr << "+Guard, -Action, +NextState, -Exit, -Entry\n";
        using fsm_statelist = get_state_list_t<std::decay_t<FsmT>>;
        if (TTraits::guard()) {
            std::forward<FsmT>(fsm).state_ = state_index_v<fsm_statelist, Next_state<TTraits>>;
        }
    }
};

// Guard, No Action, NextState, Exit, No Entry
template<typename TTraits>
struct transition<TTraits, true, false, true, true, false> {
    template<typename FsmT, typename State>
    constexpr inline void operator()(FsmT&& fsm, State&& state) const noexcept
    {
        static_assert(is_valid_guard_v<decltype(TTraits::guard)>);
        std::cerr << "+Guard, -Action, +NextState, +Exit, -Entry\n";
        using fsm_statelist = get_state_list_t<std::decay_t<FsmT>>;
        if (TTraits::guard()) {
            std::forward<State>(state).exit();
            std::forward<FsmT>(fsm).state_ = state_index_v<fsm_statelist, Next_state<TTraits>>;
        }
    }
};

// Guard, No Action, NextState, Exit, Entry
template<typename TTraits>
struct transition<TTraits, true, false, true, true, true> {
    template<typename FsmT, typename State>
    constexpr inline void operator()(FsmT&& fsm, State&& state) const noexcept
    {
        static_assert(is_valid_guard_v<decltype(TTraits::gaurd)>);
        std::cerr << "+Guard, -Action, +NextState, +Exit, +Entry\n";
        using fsm_statelist = get_state_list_t<std::decay_t<FsmT>>;
        if (TTraits::guard()) {
            std::forward<State>(state).exit();
            constexpr auto next_state_idx = state_index_v<fsm_statelist, Next_state<TTraits>>;
            std::forward<FsmT>(fsm).state_ = next_state_idx;
            Get<next_state_idx>(std::forward<FsmT>(fsm)).entry();
        }
    }
};

// Guard, Action, NextState, No Exit, No Entry
template<typename TTraits>
struct transition<TTraits, true, true, true, false, false> {
    template<typename FsmT, typename State>
    constexpr inline void operator()(FsmT&& fsm, State&&) const noexcept
    {
        static_assert(is_valid_guard_v<decltype(TTraits::guard)>);
        static_assert(is_valid_action_v<decltype(TTraits::action)>);
        std::cerr << "+Guard, +Action, +NextState, -Exit, -Entry\n";
        using fsm_statelist = get_state_list_t<std::decay_t<FsmT>>;
        if (TTraits::guard()) {
            TTraits::action();
            std::forward<FsmT>(fsm).state_ = state_index_v<fsm_statelist, Next_state<TTraits>>;
        }
    }
};

// Guard, Action, NextState, Exit, No Entry
template<typename TTraits>
struct transition<TTraits, true, true, true, true, false> {
    template<typename FsmT, typename State>
    constexpr inline void operator()(FsmT&& fsm, State&& state) const noexcept
    {
        static_assert(is_valid_guard_v<decltype(TTraits::guard)>);
        static_assert(is_valid_action_v<decltype(TTraits::guard)>);
        std::cerr << "+Guard, +Action, +NextState, +Exit, -Entry\n";
        using fsm_statelist = get_state_list_t<std::decay_t<FsmT>>;
        // if (has_guard_v<TTraits> && TTraits::guard() == false) {
        //     return;
        // }
        if (TTraits::guard()) {
            std::forward<State>(state).exit();
            TTraits::action();
            std::forward<FsmT>(fsm).state_ = state_index_v<fsm_statelist, Next_state<TTraits>>;
        }
    }
};

// Guard, Action, NextState, Exit, Entry
template<typename TTraits>
struct transition<TTraits, true, true, true, true, true> {
    template<typename FsmT, typename State>
    constexpr inline void operator()(FsmT&& fsm, State&& state) const noexcept
    {
        static_assert(is_valid_guard_v<decltype(TTraits::guard)>);
        static_assert(is_valid_action_v<decltype(TTraits::action)>);
        std::cerr << "+Guard, +Action, +NextState, +Exit, +Entry\n";
        using fsm_statelist = get_state_list_t<std::decay_t<FsmT>>;
        if (TTraits::guard()) {
            std::forward<State>(state).exit();
            TTraits::action();
            constexpr auto next_state_idx = state_index_v<fsm_statelist, Next_state<TTraits>>;
            std::forward<FsmT>(fsm).state_ = next_state_idx;
            Get<next_state_idx>(std::forward<FsmT>(fsm)).entry();
        }
    }
};

// No Guard, Action, No NextState, ?Exit, ?Entry
template<typename TTraits, bool HasExit, bool HasEntry>
struct transition<TTraits, false, true, false, HasExit, HasEntry> {
    template<typename FsmT, typename State>
    constexpr inline void operator()(FsmT&&, State&&) const noexcept
    {
        static_assert(is_valid_action_v<decltype(TTraits::action)>);
        std::cerr << "-Guard, +Action, -NextState, ?Exit, ?Entry\n";
        TTraits::action();
    }
};

// No Guard, Action, NextState, No Exit, No Entry
template<typename TTraits>
struct transition<TTraits, false, true, true, false, false> {
    template<typename FsmT, typename State>
    constexpr inline void operator()(FsmT&& fsm, State&&) const noexcept
    {
        static_assert(is_valid_action_v<decltype(TTraits::action)>);
        std::cerr << "-Guard, +Action, +NextState, -Exit, -Entry\n";
        using fsm_statelist = get_state_list_t<std::decay_t<FsmT>>;
        TTraits::action();
        std::forward<FsmT>(fsm).state_ = state_index_v<fsm_statelist, Next_state<TTraits>>;
    }
};

// No Guard, Action, NextState, Exit, No Entry
template<typename TTraits>
struct transition<TTraits, false, true, true, true, false> {
    template<typename FsmT, typename State>
    constexpr inline void operator()(FsmT&& fsm, State&& state) const noexcept
    {
        static_assert(is_valid_action_v<decltype(TTraits::action)>);
        std::cerr << "-Guard, +Action, +NextState, +Exit, -Entry\n";
        using fsm_statelist = get_state_list_t<std::decay_t<FsmT>>;
        std::forward<State>(state).exit();
        TTraits::action();
        std::forward<FsmT>(fsm).state_ = state_index_v<fsm_statelist, Next_state<TTraits>>;
    }
};

// No Guard, Action, NextState, Exit, Entry
template<typename TTraits>
struct transition<TTraits, false, true, true, true, true> {
    template<typename FsmT, typename State>
    constexpr inline void operator()(FsmT&& fsm, State&& state) const noexcept
    {
        static_assert(is_valid_action_v<decltype(TTraits::action)>);
        std::cerr << "-Guard, +Action, +NextState, +Exit, +Entry\n";
        using fsm_statelist = get_state_list_t<std::decay_t<FsmT>>;
        std::forward<State>(state).exit();
        TTraits::action();
        constexpr auto next_state_idx = state_index_v<fsm_statelist, Next_state<TTraits>>;
        std::forward<FsmT>(fsm).state_ = next_state_idx;
        Get<next_state_idx>(std::forward<FsmT>(fsm)).entry();
    }
};

// dispatch_event using `transition` for branching during event handling
template<typename FsmT, typename Event, size_type Idx, size_type... Idxs>
constexpr inline void
dispatch_event(FsmT&& fsm, Event&& event, Index_sequence<Idx,Idxs...>) noexcept
{
    if (Idx == fsm.state_) {
        using state_t = std::decay_t<state_at<Idx, FsmT>>;
        using event_t = std::decay_t<Event>;
        using transition_traits = transition_table<state_t, event_t>;

        auto&& state = Get<Idx>(std::forward<FsmT>(fsm));
        state.handle_event(std::forward<Event>(event));
        transition<transition_traits>{}(
            std::forward<FsmT>(fsm),std::forward<decltype(state)>(state));
    }
    else if constexpr (sizeof...(Idxs) != 0) {
        dispatch_event(
            std::forward<FsmT>(fsm), std::forward<Event>(event), Index_sequence<Idxs...>{});
    }
}

/* 
// Standalone dispatch_event
// 1. Evaluate the guard condition associated with the transition and perform the following steps
//    only if the guard evaluates to TRUE.
// 2. Exit the source state configuration.
// 3. Execute the actions associated with the transition.
// 4. Enter the target state configuration.
template<typename FsmT, typename Event, size_type Idx, size_type... Idxs>
constexpr inline void
dispatch_event(FsmT&& fsm, Event&& event, Index_sequence<Idx,Idxs...>) noexcept
{
    if (Idx == fsm.state_) {
        using state_t = std::decay_t<state_at<Idx, FsmT>>;
        using event_t = std::decay_t<Event>;
        // using statelist_t = typename std::decay_t<FsmT>::State_list;
        using fsm_statelist = get_state_list_t<std::decay_t<FsmT>>;
        using transition_traits = transition_table<state_t, event_t>;

        auto&& state = Get<Idx>(std::forward<FsmT>(fsm));
        state.handle_event(std::forward<Event>(event));

        if constexpr (has_guard_v<transition_traits>) {
            static_assert(is_valid_guard_v<decltype(transition_traits::guard)>);
            std::cerr << "Guard branch\n";
            if (transition_traits::guard()) {
                if constexpr (has_exit_v<transition_traits> &&
                              has_next_state_v<transition_traits>) {
                    state.exit();
                }
                // ideally we should move to a "non-state" state
                if constexpr (has_action_v<transition_traits>) {
                    static_assert(is_valid_action_v<decltype(transition_traits::action)>);
                    std::cerr << "Action under Guard branch\n";
                    transition_traits::action();
                }
                if constexpr (has_next_state_v<transition_traits>) {
                    constexpr auto next_state_idx =
                        state_index_v<fsm_statelist, Next_state<transition_traits>>;
                    // fsm.state_ = state_index_v<fsm_statelist, typename transition_traits::next_state>;
                    fsm.state_ = next_state_idx;
                    if constexpr (has_entry_v<transition_traits>) {
                        Get<next_state_idx>(std::forward<FsmT>(fsm)).entry();
                    }
                }
            }
        }
        else if constexpr (has_action_v<transition_traits>) {
            std::cerr << "Action branch\n";
            if constexpr (has_exit_v<transition_traits> &&
                          has_next_state_v<transition_traits>) {
                state.exit();
            }
            transition_traits::action();
            if constexpr (has_next_state_v<transition_traits>) {
                constexpr auto next_state_idx =
                    state_index_v<fsm_statelist, Next_state<transition_traits>>;
                fsm.state_ = next_state_idx;
                if constexpr (has_entry_v<transition_traits>) {
                    Get<next_state_idx>(std::forward<FsmT>(fsm)).entry();
                }
            }
        }
        else {
            std::cerr << "No Guard, no Action branch\n";
            if constexpr (has_next_state_v<transition_traits>) {
                constexpr auto next_state_idx =
                    state_index_v<fsm_statelist, Next_state<transition_traits>>;
                fsm.state_ = next_state_idx;
                if constexpr (has_entry_v<transition_traits>) {
                    Get<next_state_idx>(std::forward<FsmT>(fsm)).entry();
                }
            }
        }
    }
    else if constexpr (sizeof...(Idxs) != 0) {
        dispatch_event(
            std::forward<FsmT>(fsm), std::forward<Event>(event), Index_sequence<Idxs...>{});
    }
    else {
        // static_assert(always_false_v<identity<Index_sequence<Idxs...>>>);
    }
}
 */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* --------------------------------------------------------------------------------------------- */
