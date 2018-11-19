#include <iostream>
#include "fsm.hpp"
#include "Stopwatch/stopwatch.h"


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
    // template<typename Event>
    // inline void handle_event(Animation&,Event) const noexcept;

    void entry(Animation const& fsm) const;
    // {
    //     std::cerr << "\tAnimating::entry, counter = " << fsm.counter << "\n";
    // }
};
struct sPaused {
    // template<typename Event>
    // inline void handle_event(Animation&,Event) const noexcept;

    void entry(Animation const& fsm) const;
    // {
    //     std::cerr << "\tPaused::entry, counter = " << fsm.counter << "\n";
    // }
};
struct sIdle {
    // template<typename Event>
    // constexpr void handle_event(Animation&,Event) const noexcept { }

    inline void entry(Animation& fsm) noexcept;
};

class Animation : public Fsm<Animation, sAnimating, sPaused, sIdle>
{
public:
    using Base = Fsm<Animation, sAnimating, sPaused, sIdle>;
    using Base::Base;
// private:
    unsigned counter{0};
    static constexpr inline unsigned counter_limit{42};
};

inline void sAnimating::handle_event(Animation& fsm, eUpdate) const noexcept
{
    ++fsm.counter;
    std::cerr << "\t" << __PRETTY_FUNCTION__ << ", counter = " << fsm.counter << "\n";
}

// template<typename Event>
// inline void sAnimating::handle_event(Animation& fsm,Event) const noexcept
// {
//     std::cerr << "\t" << __PRETTY_FUNCTION__ << ", counter = " << fsm.counter << "\n";
// }

// template<typename Event>
// inline void sPaused::handle_event(Animation& fsm,Event) const noexcept
// {
//     std::cerr << "\t" << __PRETTY_FUNCTION__ << ", counter = " << fsm.counter << "\n";
// }

inline void sIdle::entry(Animation& fsm) noexcept
{
    fsm.counter = 0;
    std::cerr << "\tIdle::entry, counter = " << fsm.counter << "\n";
}

void sAnimating::entry(Animation const& fsm) const
{
    std::cerr << "\tAnimating::entry, counter = " << fsm.counter << "\n";
}

void sPaused::entry(Animation const& fsm) const
{
    std::cerr << "\tPaused::entry, counter = " << fsm.counter << "\n";
}


template<typename Event>struct transition_table<sIdle,Event> { };
template<> struct transition_table<sIdle,ePlay> {
    using next_state = sAnimating;
};

template<typename Event> struct transition_table<sAnimating,Event> { };
template<> struct transition_table<sAnimating,eUpdate> {
    using next_state = sIdle;
    static inline const auto guard =
        [limit=Animation::counter_limit](Animation const& fsm)noexcept{
                                        return fsm.counter >= limit;
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
        eStop{},
        ePlay{},
        eUpdate{},
        eStop{}
        );
}

int main()
{
    // auto animation = Animation{initial_state_v<sIdle>};  // set initial state on construction
    std::cerr << "has_guard = " << has_guard_v<transition_table<sAnimating,eUpdate>> << "\n";
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
