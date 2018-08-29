#include <iostream>
#include <variant>
#include "Stopwatch/stopwatch.h"
#include "sml.hpp"

namespace sml = boost::sml;

// logging the state machine operating - a class with the following set of templates
// needs to be defined
struct my_logger
{
    template<typename SM, typename Event>
    void log_process_event(const Event&) noexcept
    {
        std::cout << "[" << sml::aux::get_type_name<SM>()
            << "][event]: " << sml::aux::get_type_name<Event>() << "\n";
    }

    template<typename SM, typename Guard, typename Event>
    void log_guard(const Guard&, const Event&, bool result) noexcept
    {
        std::cout << "[" << sml::aux::get_type_name<SM>()
            << "][guard]: " << sml::aux::get_type_name<Guard>()
            << ", " << sml::aux::get_type_name<Event>()
            << ": " << (result ? "[OK]" : "[Reject]") << "\n";
    }

    template<typename SM, typename Action, typename Event>
    void log_action(const Action&, const Event&) noexcept
    {
        std::cout << "[" << sml::aux::get_type_name<SM>()
            << "][action]: " << sml::aux::get_type_name<Action>()
            << ", " << sml::aux::get_type_name<Event>() << "\n";
    }

    template<typename SM, typename SrcState, typename DstState>
    void log_state_change(const SrcState& src, const DstState& dst) noexcept
    {
        std::cout << "[" << sml::aux::get_type_name<SM>()
            << "][transition]: " << src.c_str() << " -> " << dst.c_str() << "\n";
    }
};

// events
struct eUpdate { };
struct ePlay { };
struct ePause { };
struct eStop { };


// states

class Animation
{

struct sIdle { };
struct sAnimating { unsigned counter{}; };
struct sPaused { };

struct my_unexpected_event_handler { };
struct my_exception_handler { };

public:
    constexpr Animation() noexcept = default;

    struct instance_ref
    {
        explicit constexpr instance_ref(Animation& a) noexcept
            : obj_{a} { }
        Animation& obj_;
    };

    struct CtrGuard : instance_ref
    {
        using instance_ref::instance_ref;
        bool operator()() const noexcept
        {
            return std::get<sAnimating>(obj_.data_).counter >= counter_limit;
        }
    };

    struct UpdateAction : instance_ref
    {
        using instance_ref::instance_ref;
        void operator()() noexcept
        {
            auto& ctr = std::get<sAnimating>(obj_.data_).counter;
            ++ctr;
            // std::cout << "Animating, counter: " << ctr << "\n";

        }
    };

    inline auto operator()()
    {
        using namespace sml;

        return make_transition_table(
            * state<sIdle> + event<ePlay> /
                [this]{data_.emplace<sAnimating>().counter = 0;} = state<sAnimating>

            , state<sAnimating> + event<eUpdate> /* [ !animating_guard ] */ / update_action
            , state<sAnimating> + event<eUpdate> [ animating_guard ] = state<sIdle>
            , state<sAnimating> + event<ePause> = state<sPaused>
            , state<sAnimating> + event<eStop>  = state<sIdle>

            , state<sPaused>    + event<ePlay> = state<sAnimating>
            , state<sPaused>    + event<eStop> = state<sIdle>

// error handling
            // ,*state<my_unexpected_event_handler> + unexpected_event<eUpdate> /
            //     []{ std::cout << "unexpected Update event\n"; } /* = X // could have a transition here*/
            ,*state<my_unexpected_event_handler> + unexpected_event<_> /
                []{ std::cout << "catch-all unexpected event\n"; } /* = state<some_state> */

// the SM can also catch any thrown exceptions
            ,*state<my_exception_handler> + exception<std::runtime_error> /
                []{std::cout << "runtime exception caught\n"; } /* = state<some_state // could transition */
            ,*state<my_exception_handler> + exception<_> /
                []{std::cout << "generic exception caught\n"; } /* = X */
        );
    }

// private:
// sAnimating, sPaused data
    std::variant<sIdle, sAnimating, sPaused> data_{};

    const CtrGuard animating_guard{*this};
    UpdateAction update_action{*this};
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
    // sml::sm<Animation> sm{};
    // constexpr auto num_laps = 1'000'000u;
    // auto sw = Stopwatch{"Animation"};
    // for(auto i = 0u; i < num_laps; ++i){
    //     run_animation(sm);
    // }
    // sw.stop();
    // const auto total = sw.lap_get();
    // std::cout << "Animation benchmark over " << num_laps << " iterations:\n"
    //     << "-   total = " << total << " ms\n"
    //     << "-   average per 10 transitions = " << static_cast<double>(total) / num_laps << " ms\n"
    //     << "-   average per transition = " << static_cast<double>(total) / num_laps /10 << " ms\n";

// instantiating the state machine with a logger
    auto logger = my_logger{};
    sml::sm<Animation, sml::logger<my_logger>> sm{logger};
    sm.process_event(ePlay{});
    sm.process_event(eUpdate{});
    sm.process_event(ePause{});
    sm.process_event(ePlay{});
    sm.process_event(eUpdate{});
    sm.process_event(ePause{});
    sm.process_event(eUpdate{});
    sm.process_event(eStop{});
}
