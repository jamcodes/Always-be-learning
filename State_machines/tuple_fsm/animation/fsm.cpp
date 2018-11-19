#include <iostream>
#include "fsm.hpp"
#include "Stopwatch/stopwatch.h"

unsigned counter{0};
static constexpr inline unsigned counter_limit{42};

struct ePlay { };
struct eStop { };
struct ePause { };
struct eUpdate { };

struct sAnimating;
struct sPaused;
struct sIdle;


class Animation;

struct sAnimating {
    inline void handle_event(Animation& fsm, eUpdate) const noexcept;
    // {
    //     ++fsm.counter;
    // }
    // template<typename Event>
    // constexpr void handle_event(Event&&) const noexcept { }

    void entry(Animation&) const {
        std::cerr << "Animating, counter = " << counter << "\n";
    }
};
struct sPaused {
    // template<typename Event>
    // constexpr void handle_event(Event&&) const noexcept { }

    void entry(Animation&) const {
        std::cerr << "Paused, counter = " << counter << "\n";
    }
};
struct sIdle {
    // template<typename Event>
    // constexpr void handle_event(Event&&) const noexcept { }

    inline void entry(Animation& fsm) noexcept;
    // {
    //     fsm.counter = 0;
        // std::cerr << "Idle, counter = " << counter << "\n";
    // }
};

class Animation : public Fsm<sAnimating, sPaused, sIdle>
{
public:
    using Base = Fsm<sAnimating, sPaused, sIdle>;
    using Base::Base;
// private:
    unsigned counter{0};
    static constexpr inline unsigned counter_limit{42};
};

inline void sAnimating::handle_event(Animation& fsm, eUpdate) const noexcept
{
    ++fsm.counter;
}

inline void sIdle::entry(Animation& fsm) noexcept
{
    fsm.counter = 0;
    std::cerr << "Idle, counter = " << counter << "\n";
}

template<typename Event>struct transition_table<sIdle,Event> { };
template<> struct transition_table<sIdle,ePlay> {
    using next_state = sAnimating;
};

template<typename Event> struct transition_table<sAnimating,Event> { };
template<> struct transition_table<sAnimating,eUpdate> {
    using next_state = sIdle;
    static inline const auto guard = [limit=counter_limit, ctr=counter]()noexcept{
                                            return ctr >= counter_limit;
                                         };
};
template<> struct transition_table<sAnimating,ePause> {
    using next_state = sPaused;
};
template<> struct transition_table<sAnimating,eStop> {
    using next_state = sIdle;
};

template<typename Event> struct transition_table<sPaused,Event> { };
template<> struct transition_table<sPaused,ePlay> {
    using next_state = sAnimating;
};
template<> struct transition_table<sPaused,eStop> {
    using next_state = sIdle;
};

// using Animation = Fsm<sAnimating, sPaused, sIdle>;

template<typename SM, typename... Events>
inline void send_events(SM&& fsm, Events&&... events) noexcept
{
    ( ... , fsm.dispatch_event(std::forward<Events>(events)) );
}

template<typename T>
inline void run_animation(T&& fsm) noexcept
{
    send_events(std::forward<T>(fsm),
        ePlay{},
        eUpdate{},
        ePause{},
        eUpdate{},
        ePlay{},
        eUpdate{},
        // eStop{},
        ePlay{},
        eUpdate{}
        // eStop{}
        );
}

int main()
{
    // auto animation = Animation{initial_state_v<sIdle>};  // set initial state on construction
    auto animation = Animation{};
    animation.set_initial_state(initial_state_v<sIdle>);    // or explicitly later
    // constexpr auto num_laps = 1'000'000u;
    // auto sw = Stopwatch{"Animation"};
    // for (auto i=0u; i<num_laps; ++i) {
    //     run_animation(animation);
    // }
    // sw.stop();
    // const auto total = sw.lap_get();
    // std::cout << "Animation benchmark over " << num_laps << " iterations:\n"
    //     << "-   total = " << total << " ms\n"
    //     << "-   average per 10 transitions = " << static_cast<double>(total) / num_laps << " ms\n"
    //     << "-   average per transition = " << static_cast<double>(total) / num_laps /10 << " ms\n";

    run_animation(animation);
}
