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
};
struct sPaused {
    // template<typename Event>
    // inline void handle_event(Animation&,Event) const noexcept;

    void entry(Animation const& fsm) const;
};
struct sIdle {
    // template<typename Event>
    // constexpr void handle_event(Animation&,Event) const noexcept { }

    inline void entry(Animation& fsm) noexcept;
};

struct animation_logger {
    template<typename State, typename Event>
    void log_event(Animation const&, State const&, Event const&) const noexcept
    {
        std::cerr << '[' << logging::get_type_name<Animation>()
            << "]: in state [" << logging::get_type_name<State>()
            << "], got event [" << logging::get_type_name<Event>() << "]\n";
    }
    template<typename Guard>
    void log_guard(Animation const&, Guard const&, bool result) const noexcept
    {
        std::cerr << "[" << logging::get_type_name<Animation>()
            << "]: guard [" << logging::get_type_name<Guard>() << "]"
            << (result ? " [OK]" : " [Reject]") << "\n";
    }
    template<typename Action>
    void log_action(Animation const&, Action const&) const noexcept
    {
        std::cerr << "[" << logging::get_type_name<Animation>()
            << "]: action [" << logging::get_type_name<Action>() << "]\n";
    }
    template<typename State>
    void log_exit(Animation const&, State const&) const noexcept
    {
        std::cerr << "[" << logging::get_type_name<Animation>()
            << "]: state exit [" << logging::get_type_name<State>() << "]\n";
    }
    template<typename State>
    void log_entry(Animation const&, State const&) const noexcept
    {
        std::cerr << "[" << logging::get_type_name<Animation>()
            << "]: state entry [" << logging::get_type_name<State>() << "]\n";
    }
    template<typename SrcState, typename DstState>
    void log_state_change(Animation const&, SrcState const&, DstState const&) const noexcept
    {
        std::cerr << "[" << logging::get_type_name<Animation>()
            << "]: state change [" << logging::get_type_name<SrcState>() << "] -> ["
            << logging::get_type_name<DstState>() << "]\n";
    }
};

class Animation : public Fsm<Animation, sAnimating, sPaused, sIdle>
{
public:
    using Base = Fsm<Animation, sAnimating, sPaused, sIdle>;
    using Base::Base;

    static constexpr inline animation_logger& logger() noexcept
    {
        return Animation::logger_;
    }

    // static constexpr inline auto const& transition_table() noexcept { return transition_table_; }

// --- data members
    unsigned counter{0};
    static constexpr inline unsigned counter_limit{42};

    // constexpr auto guard1 = [](Animation const&) noexcept { return true; };
    // constexpr auto action1 = [](Animation const&) noexcept { std::cerr << __PRETTY_FUNCTION__ << "\n" };
    struct guard1 {
        constexpr bool operator()(Animation const&) const noexcept {return true;}
    };
    struct action1 {
        void operator()(Animation const&) const {
            // std::cerr << __PRETTY_FUNCTION__ << "\n";
        }
    };
    struct guard2 {
        constexpr bool operator()(Animation const& fsm) const noexcept {
            return fsm.counter >= fsm.counter_limit;
        }
    };
    static inline constexpr TransitionTraits<
        TransitionEntry<sIdle,ePlay,sAnimating,guard1,action1>,
        TransitionEntry<sAnimating,eUpdate,sIdle,guard2,void>,
        TransitionEntry<sAnimating,ePause,sPaused,void,void>,
        TransitionEntry<sAnimating,eStop,sIdle,void,void>,
        TransitionEntry<sPaused,ePlay,sAnimating,void,void>,
        TransitionEntry<sPaused,eStop,sIdle,void,void>
    > transition_table{};

    // static inline constexpr decltype(auto) transition_table() noexcept
    // {
    //     return make_transition_table(
    //         transition_entry(state<sIdle>, event<ePlay>, next_state<sAnimating>, guard<guard1>, action<action1>),

    //     );
    // }
private:
    static inline animation_logger logger_{};
};

inline void sAnimating::handle_event(Animation& fsm, eUpdate) const noexcept
{
    ++fsm.counter;
    // std::cerr << "\t" << __PRETTY_FUNCTION__ << ", counter = " << fsm.counter << "\n";
}

// template<typename Event>
// inline void sAnimating::handle_event(Animation& fsm,Event) const noexcept
// {
    // std::cerr << "\t" << __PRETTY_FUNCTION__ << ", counter = " << fsm.counter << "\n";
// }

// template<typename Event>
// inline void sPaused::handle_event(Animation& fsm,Event) const noexcept
// {
    // std::cerr << "\t" << __PRETTY_FUNCTION__ << ", counter = " << fsm.counter << "\n";
// }

inline void sIdle::entry(Animation& fsm) noexcept
{
    fsm.counter = 0;
    // std::cerr << "\tIdle::entry, counter = " << fsm.counter << "\n";
}

void sAnimating::entry(Animation const& fsm) const
{
    // std::cerr << "\tAnimating::entry, counter = " << fsm.counter << "\n";
}

void sPaused::entry(Animation const& fsm) const
{
    // std::cerr << "\tPaused::entry, counter = " << fsm.counter << "\n";
}

struct testguard1 {
    inline bool operator()(Animation const&) const{
        std::cerr << "testguard1\n";
        return true;
    }
};

struct testguard2 {
    inline bool operator()(Animation const&) const{
        std::cerr << "testguard2\n";
        return false;
    }
};

struct testaction {
    inline void operator()(Animation const&) const {
        std::cerr << "testaction\n";
    }
};

using te1 = TransitionEntry<sIdle,ePlay,sAnimating,testguard1,testaction>;
using te2 = TransitionEntry<sAnimating,eUpdate,sIdle,testguard1,void>;
using te3 = TransitionEntry<sAnimating,ePause,sPaused,void,testaction>;

static_assert(has_guard_v<te1,Animation&>);
static_assert(has_guard_v<te2,Animation&>);
static_assert(!has_guard_v<te3,Animation&>);
static_assert(has_action_v<te1,Animation&>);
static_assert(!has_action_v<te2,Animation&>);
static_assert(has_action_v<te3,Animation&>);

// template<typename Event>struct transition_table<sIdle,Event> { };
// template<> struct transition_table<sIdle,ePlay> {
//     using next_state = sAnimating;
//     static inline const auto guard = [](Animation const&)noexcept {return true;};
//     static inline const auto action = [](Animation const& fsm) noexcept {
//         // std::cerr << "{sIdle->sAnimating}::action: " << fsm.counter << "\n";
//     };
// };

// template<typename Event> struct transition_table<sAnimating,Event> { };
// template<> struct transition_table<sAnimating,eUpdate> {
//     using next_state = sIdle;
//     static inline const auto guard =
//         [limit=Animation::counter_limit](Animation const& fsm) noexcept {
//             return fsm.counter >= limit;
//         };
// };
// template<> struct transition_table<sAnimating,ePause> {
//     using next_state = sPaused;
// };
// template<> struct transition_table<sAnimating,eStop> {
//     using next_state = sIdle;
// };

// template<typename Event> struct transition_table<sPaused,Event> { };
// template<> struct transition_table<sPaused,ePlay> {
//     using next_state = sAnimating;
//     static inline const auto action = [](Animation const& fsm){
//         // std::cerr << "{sPaused->sAnimating}::action: " << fsm.counter << "\n";
//     };
// };
// template<> struct transition_table<sPaused,eStop> {
//     using next_state = sIdle;
// };

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
    // std::cerr << "has_guard = " << has_guard_v<transition_table<sAnimating,eUpdate>> << "\n";
    auto animation = Animation{};
    animation.set_initial_state(initial_state_v<sIdle>);    // or explicitly later
#if !defined(FSM_DEBUG_LOG)
    constexpr auto num_laps = 1'000'000u;
    auto sw = Stopwatch{"Animation"};
    for (auto i=0u; i<num_laps; ++i) {
        run_animation(animation);
    }
    sw.stop();
    const auto total = sw.lap_get();
    std::cout << "Animation benchmark over " << num_laps << " iterations:\n"
        << "-   total = " << total << " ms\n"
        << "-   average per 10 transitions = " << static_cast<double>(total) / num_laps << " ms\n"
        << "-   average per transition = " << static_cast<double>(total) / num_laps /10 << " ms\n";
#else
    run_animation(animation);
#endif
}
