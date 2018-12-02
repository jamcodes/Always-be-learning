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
    template<typename Action>
    constexpr inline auto add_action(Action&& action) noexcept
    {
        return TransitionEntry<State,Event,NextState,void,std::decay_t<Action>>{
            std::forward<Action>(action)};
    }
    template<typename Guard>
    constexpr inline auto add_guard(Guard&& guard) noexcept
    {
        return TransitionEntry<State,Event,NextState,std::decay_t<Guard>,void>{
            std::forward<Guard>(guard)};
    }
};

template<typename State, typename Event, typename NextState, typename Guard>
struct TransitionEntry<State, Event, NextState, Guard, void> {
    using next_state = NextState;
    Guard guard{};
    template<typename Action>
    constexpr inline auto add_action(Action&& action) noexcept
    {
        return TransitionEntry<State,Event,NextState,Guard,std::decay_t<Action>>{
            std::move(guard), std::forward<Action>(action)};
    }
};

template<typename State, typename Event, typename NextState, typename Action>
struct TransitionEntry<State, Event, NextState, void, Action> {
    using next_state = NextState;
    Action action{};
    template<typename Guard>
    constexpr inline auto add_guard(Guard&& guard) noexcept
    {
        return TransitionEntry<State,Event,NextState,std::decay_t<Guard>,Action>{
            std::forward<Guard>(guard), std::move(action)
        };
    }
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


template<typename... Entries>
constexpr auto make_transition_table(Entries&&... entries) noexcept
{
    return TransitionTraits<Entries...>{std::forward<Entries>(entries)...};
}

template<typename T> struct wrap_t { using type = T; };
template<typename T> constexpr inline auto wrap = wrap_t<T>{};

template<typename State, typename Event, typename NextState, typename Guard, typename Action>
constexpr auto make_entry(wrap_t<State>, wrap_t<Event>, wrap_t<NextState>,
                          Guard guard, Action action) noexcept
{
    return TransitionEntry<State,Event,NextState,Guard,Action>{std::move(guard), std::move(action)};
}

template<typename State, typename Event, typename NextState>
constexpr auto make_entry(wrap_t<State>, wrap_t<Event>, wrap_t<NextState>) noexcept
{
    return TransitionEntry<State,Event,NextState,void,void>{};
}

template<typename State, typename Event, typename NextState, typename Guard>
constexpr auto make_gentry(wrap_t<State>, wrap_t<Event>, wrap_t<NextState>,
                          Guard guard) noexcept
{
    return TransitionEntry<State,Event,NextState,Guard,void>{std::move(guard)};
}

template<typename State, typename Event, typename NextState, typename Action>
constexpr auto make_aentry(wrap_t<State>, wrap_t<Event>, wrap_t<NextState>,
                          Action action) noexcept
{
    return TransitionEntry<State,Event,NextState,void,Action>{std::move(action)};
}
