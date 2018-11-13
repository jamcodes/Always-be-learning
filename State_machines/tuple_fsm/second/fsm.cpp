#include <iostream>
#include "fsm.hpp"

struct EventA { };
struct EventB { };

struct StateA {
    void handle_event(EventA const&) noexcept
    {
        std::cout << "\t" << __PRETTY_FUNCTION__ << "\n";
    }
    void handle_event(EventB const&) noexcept
    {
        std::cout << "\t" << __PRETTY_FUNCTION__ << "\n";
    }
};
struct StateB {
    void handle_event(EventA const&) noexcept
    {
        std::cout << "\t" << __PRETTY_FUNCTION__ << "\n";
    }
    void handle_event(EventB const&) noexcept
    {
        std::cout << "\t" << __PRETTY_FUNCTION__ << "\n";
    }
};
struct StateC {
    void handle_event(EventA const&) noexcept
    {
        std::cout << "\t" << __PRETTY_FUNCTION__ << "\n";
    }
    void handle_event(EventB const&) noexcept
    {
        std::cout << "\t" << __PRETTY_FUNCTION__ << "\n";
    }
};

using StateMachine = Fsm<StateA,StateB,StateC>;
template<typename Event>
struct transition_table<StateA, Event> {
    using next_state = StateB;
    static constexpr inline auto action = [](){ std::cerr << "\tStateA fallback Event action\n"; };
};
template<>
struct transition_table<StateA, EventA> {
    static constexpr inline auto action = []() { std::cerr << "\tStateA EventA action\n"; };
};
template<typename Event>
struct transition_table<StateB, Event> {
    using next_state = StateC;
    static constexpr inline auto action = [](){ std::cerr << "\tStateB any Event action\n"; };
    static constexpr inline auto guard =
        []()noexcept -> bool { std::cerr << "\tStateB Guard\n"; return true; };
};
template<typename Event>
struct transition_table<StateC, Event> {
    using next_state = StateA;
    static constexpr inline auto action = [](){ std::cerr << "\tStateC any Event action\n"; };
    static constexpr inline auto guard =
        []()noexcept -> bool { std::cerr << "\tStateC Guard\n"; return true; };
};

int main()
{
    StateMachine fsm(StateA{}, StateB{}, StateC{});
    fsm.dispatch_event(EventA{});
    fsm.dispatch_event(EventA{});
    fsm.dispatch_event(EventB{});
    fsm.dispatch_event(EventB{});
    fsm.dispatch_event(EventB{});
    // auto sa = Get<0>(fsm);
}
