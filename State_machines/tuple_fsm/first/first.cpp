#include <iostream>
#include "first.hpp"


struct EventA { };
struct EventB { };

struct StateA
{
    void handle_event(EventA const&) noexcept
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
    }

    void handle_event(EventB const&) noexcept
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
    }

};

struct StateB
{
    void handle_event(EventA const&) noexcept
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
    }

    void handle_event(EventB const&) noexcept
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
    }
};

using StateMachine = Fsm<StateA, StateB>;

struct Guard { bool operator()() const noexcept { return true; } };

template<> struct transition_table<StateA,EventA> {
    using next_state = StateB;
    static inline const auto action = [](StateMachine&)noexcept
                                        { std::cerr << "StateA,EventA action\n"; };
    // static inline const auto guard = []()->bool{return true;};
    static inline const auto guard = Guard{};
    };
template<> struct transition_table<StateA,EventB> { using next_state = StateB; };
template<typename Event> struct transition_table<StateB,Event> { using next_state = StateA; };
// template<> struct transition_table<StateB,EventB> { using type = StateA; };



int main()
{
    Fsm<StateA, StateB> testfsm{};
    testfsm.dispatch(EventA{});
    testfsm.dispatch(EventA{});
    testfsm.dispatch(EventA{});
    testfsm.dispatch(EventB{});
}
