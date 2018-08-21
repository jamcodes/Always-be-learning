#include <iostream>
#include <string>
#include <functional>

/**
 * Transition-table based implementation of a simple FSM.
 * Each state is represented by a struct with operator() - implementing the state handler.
 * Optionally each state could support on_entry(), on_exit() actions and a guard() to determine
 * if the transition should be executed.
 * State transitions decisions are done based on a transition-table.
 * Alternatively each state struct could have a next_state() or transition() function,
 * which determins the next state.
 */

enum class Event;

template<typename StateHandler_t, typename State_t,
         unsigned Num_states, unsigned Num_events>
class Fsm
{
    using State_table_ref = StateHandler_t(&)[Num_states];
    using Transition_table_ref = const State_t(&)[Num_states][Num_events];
public:
    constexpr Fsm(StateHandler_t(&state_table)[Num_states]
                 ,const State_t(&transition_table)[Num_states][Num_events]
                 ,State_t initial) noexcept
        : current_state_{initial}
        , state_table_{state_table}
        , transition_table_{transition_table}
        { }

    constexpr void initial_state(State_t s) noexcept
    {
        current_state_ = s;
    }

    constexpr void handle(Event e) noexcept
    {
        auto& p_handler = state_table_[state_index(current_state_)];
        std::invoke(p_handler, e);
        transition(e);
    }

    constexpr void transition(Event e) noexcept
    {
        current_state_ = transition_table_[state_index(current_state_)][event_index(e)];
    }

protected:
    static constexpr auto state_index(State_t s) noexcept
    {
        return static_cast<const unsigned>(s);
    }

    static constexpr auto event_index(Event e) noexcept
    {
        return static_cast<const unsigned>(e);
    }

private:
    State_t              current_state_;
    State_table_ref      state_table_;
    Transition_table_ref transition_table_;
};

// make function
template<typename StateHandler_t, typename State_t,
         unsigned Num_states, unsigned Num_events
         >
constexpr auto make_fsm(StateHandler_t(&state_table)[Num_states],
                        State_t(&transit_table)[Num_states][Num_events],
                        State_t initial) noexcept
{
    return Fsm<StateHandler_t, State_t, Num_states, Num_events>{
            state_table, transit_table, initial};
}

// C++17 deduction guide
template<typename StateHandler_t, typename State_t,
         unsigned Num_states, unsigned Num_events
         >
Fsm(StateHandler_t(&state_table)[Num_states],
    State_t(&transit_table)[Num_states][Num_events],
    State_t init)
-> Fsm<StateHandler_t, State_t, Num_states, Num_events>;


enum class Event
{
    Play,
    Pause,
    Stop,
    Update,
    Show,

    NumEvents
};

enum class State
{
    Animating,
    Paused,
    Idle,

    NumStates
};

static constexpr auto num_events() noexcept { return static_cast<unsigned>(Event::NumEvents); }
static constexpr auto num_states() noexcept { return static_cast<unsigned>(State::NumStates); }

static constexpr auto state_index(State s) noexcept
{
    return static_cast<const unsigned>(s);
}

static constexpr auto event_index(Event e) noexcept
{
    return static_cast<const unsigned>(e);
}

using state_handler_t = void(*)(Event e) noexcept;

struct StateType
{
    void operator()(Event e) noexcept
    {
        std::invoke(handler, e);
    }
    state_handler_t handler;
    unsigned counter;
};

void handle_animating(Event e) noexcept;
void handle_paused(Event e) noexcept;
void handle_idle(Event e) noexcept;


static StateType AnimationStateHandlers[] = {
    {handle_animating, 0},
    {handle_paused, 0},
    {handle_idle, 0}
};

static constexpr State AnimationTransitions[][num_events()] = {
    // animating
    {
        State::Animating,   // Animate
        State::Paused,      // Pause
        State::Idle,        // Stop
        State::Animating,   // Update
        State::Animating,   // Show
    },
    // Paused
    {
        State::Animating,   // Animate
        State::Paused,      // Pause
        State::Idle,        // Stop
        State::Paused,      // Update
        State::Paused,      // Show
    },
    // stop
    {
        State::Animating,   // Animate
        State::Idle,        // Pause
        State::Idle,        // Stop
        State::Idle,        // Update
        State::Idle,        // Show
    }
};


int main()
{
    auto fsm = Fsm{AnimationStateHandlers, AnimationTransitions, State::Idle};
    // fsm.initial_state(State::Idle);
    for(auto i = 0u; i < 10u; ++i)
    {
        fsm.handle(Event::Show);
        fsm.handle(Event::Play);
        fsm.handle(Event::Update);
        fsm.handle(Event::Show);
        fsm.handle(Event::Pause);
        fsm.handle(Event::Update);
        fsm.handle(Event::Show);
        fsm.handle(Event::Play);
        fsm.handle(Event::Update);
        fsm.handle(Event::Show);
        fsm.handle(Event::Update);
        fsm.handle(Event::Update);
    }
    fsm.handle(Event::Stop);
    fsm.handle(Event::Pause);
    fsm.handle(Event::Update);
    fsm.handle(Event::Show);
}


void handle_animating(Event e) noexcept
{
    auto& ctr = AnimationStateHandlers[state_index(State::Animating)].counter;
    switch (e)
    {
    case Event::Play:
        break;
    case Event::Pause:
        AnimationStateHandlers[state_index(State::Paused)].counter = ctr;
        break;
    case Event::Stop:
        ctr = 0;
        break;
    case Event::Update:
        ++ctr;
        break;
    case Event::Show:
        std::cout << "Animating, count: " << ctr << "\n";
        break;
    default:
        std::cout << "Error: default reached\n";
    }
}

void handle_paused(Event e) noexcept
{
    auto& ctr = AnimationStateHandlers[state_index(State::Paused)].counter;
    switch (e)
    {
    case Event::Play:
        AnimationStateHandlers[state_index(State::Animating)].counter = ctr;
        break;
    case Event::Stop:
        ctr = 0;
        break;
    case Event::Pause:
    case Event::Update:
        break;
    case Event::Show:
        std::cout << "Paused, count: " << ctr << "\n";
        break;
    default:
        std::cout << "Error[Paused]: default reached\n";
    }
}

void handle_idle(Event e) noexcept
{
    auto& ctr = AnimationStateHandlers[state_index(State::Idle)].counter;
    switch (e)
    {
    case Event::Play:
    case Event::Pause:
    case Event::Stop:
    case Event::Update:
        break;
    case Event::Show:
        std::cout << "Idle, count: " << ctr << "\n";
        break;
    default:
        std::cout << "Error[Idle]: default reached\n";
    }
}
