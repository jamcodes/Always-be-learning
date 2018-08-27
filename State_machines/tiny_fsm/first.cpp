#include <iostream>
#include <utility>
#include <type_traits>
#include "Stopwatch/stopwatch.h"


template<typename... Ts>
struct always_false : std::false_type { };
template<typename... Ts>
constexpr inline auto always_false_v = always_false<Ts...>::value;


template<typename S, typename E>
struct Fsm_state_map
{
    static_assert(always_false_v<S,E>, "Unhandled state transition");
    using state_type = S;
};

template<typename Derived>
class Fsm
{
public:
    using fsm_type = Fsm<Derived>;
    using derived_type = Derived;
    using state_ptr_t = Derived*;

    static void set_initial_state() noexcept;

    template<typename S>
    static constexpr auto& state() noexcept
    {
        static_assert(std::is_same_v<fsm_type, typename S::fsm_type>,
            "accessing state of a different state machine");
        return state_instance<S>;
    }

    template<typename S>
    static constexpr bool is_in_state()
    {
        return current_state == &state_instance<S>;
    }

    static void reset() noexcept
    {
        Derived::reset();
    }

    static void enter() noexcept
    {
        current_state->entry();
    }

    static void start() noexcept
    {
        set_initial_state();
        enter();
    }

    template<typename E>
    static void dispatch(const E& event) noexcept
    {
        current_state->react(event);
    }

protected:

    constexpr Fsm() noexcept = default;
    friend Derived;

    template<typename S, typename E>
    using tr_evt = typename Fsm_state_map<S,E>::state_type;

    template<typename S, typename E>
    constexpr decltype(auto) transit_event() noexcept
    {
        return &state_instance<tr_evt<S,E>>;
    }

    template<typename S, typename E>
    void transit() noexcept
    {
        current_state->exit();
        current_state = transit_event<S,E>();
        current_state->entry();
    }

    template<typename S, typename E, typename Action>
    void transit(Action action) noexcept
    {
        current_state->exit();
        action();
        current_state = transit_event<S,E>();
    }

    template<typename S, typename E, typename Action, typename Condition>
    void transit(Action action, Condition condition) noexcept
    {
        if(condition()) { transit<S,E>(action); }
    }

    template<typename S>
    static inline S state_instance{};
    static state_ptr_t current_state;
};

template<typename Derived>
typename Fsm<Derived>::state_ptr_t Fsm<Derived>::current_state{};


template<typename... Fs>
struct FsmList;

template<> struct FsmList<>
{
    static void set_initial_state() { }
    static void reset() { }
    static void enter() { }
    template<typename E>
    static void dispatch(const E&) { }
};

template<typename F, typename... Fs>
struct FsmList<F, Fs...>
{
    using fsm_type = Fsm<F>;

    static void set_initial_state()
    {
        fsm_type::set_initial_state();
        FsmList<Fs...>::set_initial_state();
    }

    static void reset()
    {
        fsm_type::reset();
        FsmList<Fs...>::reset();
    }

    static void enter()
    {
        fsm_type::enter();
        FsmList<Fs...>::enter();
    }

    static void start()
    {
    // sets the initial states of ALL listed machines
        set_initial_state();
    // THEN enters() the initial state of each machine
        enter();
    }

    template<typename E>
    static void dispatch(E&& event)
    {
        fsm_type::template dispatch<E>(std::forward<E>(event));
        FsmList<Fs...>::template dispatch<E>(event);
    }
};

template<typename... Ss>
struct StateList;

template<> struct StateList<>
{
    static void reset() { }
};

template<typename S, typename... Ss>
struct StateList<S, Ss...>
{
    using fsm_type = typename S::fsm_type;
    static void reset()
    {
        fsm_type::template state_instance<S> = S{};
        StateList<Ss...>::reset();
    }
};

struct Event { };
struct EventUpdate : Event { };
struct EventPlay   : Event { };
struct EventPause  : Event { };
struct EventStop   : Event { };


class Animation : public Fsm<Animation>
{
    friend class Fsm;
public:
    constexpr Animation() noexcept = default;
    virtual ~Animation() noexcept = default;

    virtual void react(EventUpdate) noexcept;
    virtual void react(EventPlay) noexcept;
    virtual void react(EventPause) noexcept;
    virtual void react(EventStop) noexcept;

    void entry() noexcept { }
    constexpr void exit() noexcept { }
};

class StateAnimating final : public Animation
{
public:
    constexpr StateAnimating() noexcept = default;
    constexpr StateAnimating(uint32_t count) noexcept
        : counter_{count} { }

     void react(EventUpdate) noexcept final
     {
         ++counter_;
        //  std::cout << "Animating, counter = " << counter_ << "\n";
         const auto action = [this]()noexcept
            {
                counter_ = 0;
                // std::cout << "StateAnimating transition to StateIdle\n";
            };
         const auto guard = [ctr=counter_]()noexcept
            {return ctr >= StateAnimating::counter_limit;};
         transit<StateAnimating,EventUpdate>(action, guard);
     }
     void react(EventPlay) noexcept final { }
     void react(EventPause) noexcept final
     {
         transit<StateAnimating,EventPause>();
     }
     void react(EventStop) noexcept final
     {
         const auto action = [this]()noexcept{counter_ = 0;};
         transit<StateAnimating,EventStop>(action);
     }

    friend class StatePaused;
    friend class StateIdle;
private:
    static constexpr auto counter_limit{42u};
    uint32_t counter_{0};
};


class StatePaused final : public Animation
{
public:
    constexpr StatePaused() noexcept = default;
    // constexpr StatePaused(uint32_t count) noexcept
    //     : counter_{count} { }

    void react(EventUpdate) noexcept final
    {
        // std::cout << "Paused, counter = " << Animation::state<StateAnimating>().counter_ << "\n";
    }
    void react(EventPlay) noexcept final
    {
        transit<StatePaused,EventPlay>();
    }
    void react(EventPause) noexcept final { }
    void react(EventStop) noexcept final
    {
        const auto action = []()noexcept{Animation::state<StateAnimating>().counter_ = 0;};
        transit<StatePaused,EventStop>(action);
    }

// friend class StateAnimating;
// private:
//     uint32_t counter_{0};
};


struct StateIdle final : public Animation
{
    constexpr StateIdle() noexcept = default;

    void react(EventUpdate) noexcept final
    {
        // std::cout << "Idle, counter = " << Animation::state<StateAnimating>().counter_ << "\n";
    }
    void react(EventPlay) noexcept final
    {
        transit<StateIdle,EventPlay>();
    }
    void react(EventPause) noexcept final { }
    void react(EventStop) noexcept final { }
};

template<> struct Fsm_state_map <StateAnimating, EventUpdate>{ using state_type = StateIdle; };
template<> struct Fsm_state_map <StateAnimating, EventPlay>
    {
        // static_assert(always_false_v<StateAnimating,EventPlay>, "Transition not allowed");
        using state_type = StateAnimating;
    };
template<> struct Fsm_state_map <StateAnimating, EventPause>{ using state_type = StatePaused; };
template<> struct Fsm_state_map <StateAnimating, EventStop>{ using state_type = StateIdle; };

template<> struct Fsm_state_map <StatePaused, EventUpdate>{ using state_type = StatePaused; };
template<> struct Fsm_state_map <StatePaused, EventPlay>{ using state_type = StateAnimating; };
template<> struct Fsm_state_map <StatePaused, EventPause>{ using state_type = StatePaused; };
template<> struct Fsm_state_map <StatePaused, EventStop>{ using state_type = StateIdle; };

template<> struct Fsm_state_map <StateIdle, EventUpdate>{ using state_type = StateIdle; };
template<> struct Fsm_state_map <StateIdle, EventPlay>{ using state_type = StateAnimating; };
template<> struct Fsm_state_map <StateIdle, EventPause>{ using state_type = StateIdle; };
template<> struct Fsm_state_map <StateIdle, EventStop>{ using state_type = StateIdle; };

template<> void Fsm<Animation>::set_initial_state() noexcept
{
    current_state = &state_instance<StateIdle>;
}

template<typename... Es>
struct EventList;

template<> struct EventList<>
{
    static void dispatch() { }
};

template<typename E, typename... Es>
struct EventList<E, Es...>
{
    using fsm_type = Animation;
    static void dispatch()
    {
        fsm_type::template dispatch<E>(E{});
        EventList<Es...>::dispatch();
    }
};

using benchmark_events = EventList<EventPlay, EventUpdate,
                                   EventPause, EventUpdate,
                                   EventPlay, EventUpdate,
                                   EventStop, EventPlay,
                                   EventUpdate, EventStop>;

int main()
{
    auto animation = Animation{};
    animation.start();

    constexpr auto num_laps = 1'000'000u;
    auto sw = Stopwatch{"Animation"};
    for(auto i = 0u; i < num_laps; ++i){
        benchmark_events::dispatch();
    }
    sw.stop();
    const auto total = sw.lap_get();
    std::cout << "Animation benchmark over " << num_laps << " iterations:\n"
        << "-   total = " << total << " ms\n"
        << "-   average per 10 transitions = " << static_cast<double>(total) / num_laps << " ms\n"
        << "-   average per transition = " << static_cast<double>(total) / num_laps /10 << " ms\n";

    // using events = EventList<EventPlay, EventUpdate,
    //                          EventPause, EventUpdate,
    //                          EventPlay, EventUpdate,
    //                          EventUpdate, EventUpdate>;
    // for(auto i = 0u; i < 10u; ++i)
    // {
    //     events::dispatch();
    // }
    // Animation::dispatch(EventPause{});
    // Animation::dispatch(EventStop{});
    // Animation::dispatch(EventUpdate{});
}
