#pragma once

#include "traits.hpp"


template<typename State, typename Event, typename NextState = void,
         typename Guard = void, typename Action = void>
struct TransitionEntry {
    using next_state = NextState;
    Guard guard{};
    Action action{};
};

template<typename State, typename Event, typename NextState>
struct TransitionEntry<State, Event, NextState, void, void> {
    using next_state = NextState;
};

template<typename State, typename Event, typename NextState, typename Guard>
struct TransitionEntry<State, Event, NextState, Guard, void> {
    using next_state = NextState;
    Guard guard{};
};

template<typename State, typename Event, typename NextState, typename Action>
struct TransitionEntry<State, Event, NextState, void, Action> {
    using next_state = NextState;
    Action action{};
};

template<typename State, typename Event, typename Guard, typename Action>
struct TransitionEntry<State, Event, void, Guard, Action>
{
    Guard guard{};
    Action action{};
};

template<typename State, typename Event, typename Action>
struct TransitionEntry<State, Event, void, void, Action>
{
    Action action{};
};

// template<typename State, typename Event>
// struct TransitionEntry<State, Event, void, void, void> {

// };


template<typename... Entries>
struct TransitionTraits : public Entries...
{

};

template<typename State, typename Event, typename NextState, typename Guard, typename Action>
inline constexpr TransitionEntry<State,Event,NextState,Guard,Action> const&
Get_transition_traits(TransitionEntry<State,Event,NextState,Guard,Action> const& te) noexcept
{
    return te;
}

// template<typename State, typename Event, typename... Entries>
// inline constexpr decltype(auto)
// Get_transition_traits(TransitionTraits<Entries...> const& tt) noexcept
// {
//     return Get_transition_traits_impl<State,Event>(tt);
// }

template<typename State, typename Event, typename FsmT>
inline constexpr decltype(auto)
Get_fsm_ttraits(FsmT&& fsm) noexcept
{
    return Get_transition_traits<State,Event>(fsm.derived().transition_table);
}
