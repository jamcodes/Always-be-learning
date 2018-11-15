#include <iostream>
#include "fsm.hpp"

struct EventA { };
struct EventB { };

struct StateA {
    void handle_event(EventA const&) noexcept
    {
        std::cerr << "\n" << __PRETTY_FUNCTION__ << "\n";
    }
    void handle_event(EventB const&) noexcept
    {
        std::cerr << "\n" << __PRETTY_FUNCTION__ << "\n";
    }

    void entry() noexcept
    {
        std::cerr << __PRETTY_FUNCTION__ << "\n";
    }

    void exit() noexcept
    {
        std::cerr << __PRETTY_FUNCTION__ << "\n";
    }
};
struct StateB {
    void handle_event(EventA const&) noexcept
    {
        std::cerr << "\n" << __PRETTY_FUNCTION__ << "\n";
    }
    void handle_event(EventB const&) noexcept
    {
        std::cerr << "\n" << __PRETTY_FUNCTION__ << "\n";
    }

    void entry() noexcept
    {
        std::cerr << __PRETTY_FUNCTION__ << "\n";
    }

    void exit() noexcept
    {
        std::cerr << __PRETTY_FUNCTION__ << "\n";
    }
};
struct StateC {
    void handle_event(EventA const&) noexcept
    {
        std::cerr << "\n" << __PRETTY_FUNCTION__ << "\n";
    }
    void handle_event(EventB const&) noexcept
    {
        std::cerr << "\n" << __PRETTY_FUNCTION__ << "\n";
    }

    void entry() noexcept
    {
        std::cerr << __PRETTY_FUNCTION__ << "\n";
    }

    void exit() noexcept
    {
        std::cerr << __PRETTY_FUNCTION__ << "\n";
    }
};

using StateMachine = Fsm<StateA,StateB,StateC>;
template<typename Event>
struct transition_table<StateA, Event> {
    using next_state = StateB;
    static constexpr inline auto action = [](){ std::cerr << "StateA fallback Event action\n"; };
};
template<>
struct transition_table<StateA, EventA> {
    static constexpr inline auto action = []() { std::cerr << "StateA EventA action\n"; };
};
template<typename Event>
struct transition_table<StateB, Event> {
    using next_state = StateC;
    static constexpr inline auto action = [](){ std::cerr << "StateB any Event action\n"; };
    static constexpr inline auto guard =
        []()noexcept -> bool { std::cerr << "StateB Guard\n"; return true; };
};
template<typename Event>
struct transition_table<StateC, Event> {
    using next_state = StateA;
    static constexpr inline auto action = [](){ std::cerr << "StateC any Event action\n"; };
    static constexpr inline auto guard =
        []()noexcept -> bool { std::cerr << "StateC Guard\n"; return true; };
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
