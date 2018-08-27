#include <iostream>
#include <string>
#include <variant>
#include <type_traits>
#include <chrono>
#include "Stopwatch/stopwatch.h"

template<typename T>
struct always_false : std::false_type { };
template<typename T>
inline constexpr auto always_false_v = always_false<T>::value;


struct StateIdle{ };

struct StatePaused
{
    uint32_t counter_;
};

struct StateAnimating
{
    uint32_t counter_;
};

template<typename... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<typename... Ts>
overloaded(Ts&&...) -> overloaded<Ts...>;

template<typename Variant, typename... Ts>
constexpr decltype(auto) match(Variant&& variant, Ts&&... ts) noexcept
{
    return std::visit(
        overloaded{
        [](const auto&)noexcept { },  // catch-all handler, probably a bad idea to have it here?
        std::forward<Ts>(ts)...
        },
        std::forward<Variant>(variant)
        );
}

struct PlayEvent { };
struct UpdateEvent { };
struct PauseEvent { };
struct StopEvent { };

class Animation
{
public:
    using State = std::variant<StateIdle, StatePaused, StateAnimating>;
    // using Event = std::variant<UpdateEvent, PlayEvent, PauseEvent, StopEvent>;

    constexpr Animation() noexcept = default;

    // constexpr void dispatch(const Event& event) noexcept
    // {
    //     match(event,
    //         [this](const UpdateEvent& /*e*/) noexcept { this->update(); },
    //         [this](const PlayEvent& /*e*/) noexcept { this->play(); },
    //         [this](const PauseEvent& /*e*/) noexcept { this->pause(); },
    //         [this](const StopEvent& /*e*/) noexcept { this->stop(); },
    //         [](auto&& e) noexcept
    //             {static_assert(always_false_v<decltype(e)>, "Unhandled event");}
    //         );
    // }

    template<typename Event>
    constexpr void dispatch(Event&& /*event*/) noexcept
    {
        using E = std::decay_t<Event>;
        if constexpr (std::is_same_v<E, PlayEvent>) {
            play();
        }
        else if constexpr (std::is_same_v<E, UpdateEvent>) {
            update();
        }
        else if constexpr (std::is_same_v<E, PauseEvent>) {
            pause();
        }
        else if constexpr (std::is_same_v<E, StopEvent>) {
            stop();
        }
        else {
            static_assert(always_false_v<E>, "Unhandled Event");
        }
    }

    constexpr uint32_t current_count() const noexcept
    {
        return match(state_,
            [](const StateIdle& /*s*/) noexcept -> uint32_t { return 0; },
            [](const StateAnimating& s) noexcept { return s.counter_; },
            [](const StatePaused& s) noexcept { return s.counter_; },
            [](auto&& s) noexcept
                {static_assert(always_false_v<decltype(s)>, "Unhandled state"); }
            );
    }

    constexpr const char* current_state() const noexcept
    {
        return match(state_,
            [](const StateIdle&) noexcept { return "Idle"; },
            [](const StatePaused&) noexcept { return "StatePaused"; },
            [](const StateAnimating&) noexcept { return "StateAnimating"; },
            [](auto&& s) noexcept
                {static_assert(always_false_v<decltype(s)>, "Unhandled state"); }
            );
    }

    constexpr void update() noexcept
    {
        match(state_,
            [this](StateAnimating& s) noexcept
            {
                ++s.counter_;
                if(s.counter_ >= counter_limit){
                    state_ = StateIdle{};
                }
            }
            // could have a catch-all handler here, instead of in the `match` template
            // Nothing stops us from having a if-constexpr handler here, like in the first-hack
            );
    }

    constexpr void play() noexcept
    {
        match(state_,
            [this](const StateIdle&) noexcept { state_ = StateAnimating{0}; },
            [this](const StatePaused& s) noexcept { state_ = StateAnimating{s.counter_}; }
            );
    }

    constexpr void stop() noexcept
    {
        match(state_,
            [this](const StateAnimating&) noexcept {state_ = StateIdle{}; },
            [this](const StatePaused&) noexcept {state_ = StateIdle{}; }
            );
    }

    constexpr void pause() noexcept
    {
        match(state_,
            [this](const StateAnimating& s) noexcept { state_ = StatePaused{s.counter_}; }
        );
    }

private:
    static constexpr inline uint32_t counter_limit{144};
    State state_{StateIdle{}};
};


inline void run_animation(Animation& anim) noexcept
{
    anim.dispatch(PlayEvent{});
    anim.dispatch(UpdateEvent{});
    anim.dispatch(PauseEvent{});
    anim.dispatch(UpdateEvent{});
    anim.dispatch(PlayEvent{});
    anim.dispatch(UpdateEvent{});
    anim.dispatch(StopEvent{});
    anim.dispatch(PlayEvent{});
    anim.dispatch(UpdateEvent{});
    anim.dispatch(StopEvent{});
}

int main()
{
    auto fsm = Animation{};

    constexpr auto num_laps = 1'000'000u;
    auto sw = Stopwatch{"Animation"};
    for(auto i = 0u; i < num_laps; ++i){
        run_animation(fsm);
    }
    sw.stop();
    const auto total = sw.lap_get();
    std::cout << "Animation benchmark over " << num_laps << " iterations:\n"
        << "-   total = " << total << " ms\n"
        << "-   average per 10 transitions = " << static_cast<double>(total) / num_laps << " ms\n"
        << "-   average per transition = " << static_cast<double>(total) / num_laps /10 << " ms\n";

    for(auto i = 0u; i < 10u; ++i){
        std::cout << fsm.current_state() << ", count: " << fsm.current_count() << "\n";
        fsm.dispatch(PlayEvent{});
        fsm.dispatch(UpdateEvent{});
        std::cout << fsm.current_state() << ", count: " << fsm.current_count() << "\n";
        fsm.dispatch(PauseEvent{});
        fsm.dispatch(UpdateEvent{});
        std::cout << fsm.current_state() << ", count: " << fsm.current_count() << "\n";
        fsm.dispatch(PlayEvent{});
        fsm.dispatch(UpdateEvent{});
        std::cout << fsm.current_state() << ", count: " << fsm.current_count() << "\n";
        fsm.dispatch(UpdateEvent{});
        fsm.dispatch(UpdateEvent{});
    }
    fsm.dispatch(StopEvent{});
    fsm.dispatch(PauseEvent{});
    fsm.dispatch(UpdateEvent{});
    std::cout << fsm.current_state() << ", count: " << fsm.current_count() << "\n";
}
