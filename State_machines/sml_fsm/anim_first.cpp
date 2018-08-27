#include <iostream>
#include "Stopwatch/stopwatch.h"
#include "sml.hpp"

namespace sml = boost::sml;


// events
struct eUpdate { };
struct ePlay { };
struct ePause { };
struct eStop { };


// states

class Animation
{
struct sIdle { };
struct sAnimating { };
struct sPaused { };


public:
    Animation() noexcept = default;
    // explicit Animation(const unsigned& count)
    //     : counter_{count} { }

    inline auto operator()()
    {
        using namespace sml;
        const auto ctr_guard = [this]()noexcept { return counter_ >= counter_limit; };
        return make_transition_table(
            * state<sIdle> + event<ePlay> = state<sAnimating>
            , state<sIdle> + on_entry<_>  /
                [this](){counter_ = 0; /* std::cout << "Idle, counter: " << counter_ << "\n"; */}

            , state<sAnimating> + event<eUpdate> [ ctr_guard ] /
                                                    [this](){++counter_;} = state<sIdle>
            , state<sAnimating> + event<ePause> = state<sPaused>
            , state<sAnimating> + event<eStop>  / [this](){counter_ = 0;} = state<sIdle>
            // , state<sAnimating> + on_entry<_>   /
            //     [this](){std::cout << "Animating, counter: " << counter_ << "\n";}

            , state<sPaused>    + event<ePlay> = state<sAnimating>
            , state<sPaused>    + event<eStop> / [this](){counter_ = 0;} = state<sIdle>
            // , state<sPaused>    + on_entry<_>  /
            //     [this](){std::cout << "Paused, counter: " << counter_ << "\n";}
        );
    }

// private:
// sAnimating, sPaused data
    unsigned counter_{};

    constexpr static inline auto counter_limit{42};
};

template<typename SM, typename T>
inline void events(SM&& s, T&& e) noexcept
{
    s.process_event(std::forward<T>(e));
}

template<typename SM, typename T, typename... Ts>
inline void events(SM&& s, T&& e, Ts&&... es) noexcept
{
    s.process_event(std::forward<T>(e));
    events(std::forward<SM>(s), std::forward<Ts>(es)...);
}

template<typename T>
inline void run_animation(T&& s) noexcept
{
    events(std::forward<T>(s),
        ePlay{},
        eUpdate{},
        ePause{},
        eUpdate{},
        ePlay{},
        eUpdate{},
        eStop{},
        ePlay{},
        eUpdate{},
        eStop{});
}

int main()
{
    sml::sm<Animation> sm{0};

    constexpr auto num_laps = 1'000'000u;
    auto sw = Stopwatch{"Animation"};
    for(auto i = 0u; i < num_laps; ++i){
        run_animation(sm);
    }
    sw.stop();
    const auto total = sw.lap_get();
    std::cout << "Animation benchmark over " << num_laps << " iterations:\n"
        << "-   total = " << total << " ms\n"
        << "-   average per 10 transitions = " << static_cast<double>(total) / num_laps << " ms\n"
        << "-   average per transition = " << static_cast<double>(total) / num_laps /10 << " ms\n";

    sm.process_event(ePlay{});
    sm.process_event(ePause{});
    sm.process_event(ePlay{});
    sm.process_event(eUpdate{});
    sm.process_event(ePause{});
    sm.process_event(eStop{});
}
