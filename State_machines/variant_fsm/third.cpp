#include <iostream>
#include <string>
#include <variant>
#include <optional>
#include <type_traits>
#include "Stopwatch/stopwatch.h"

template<typename T>
struct always_false : std::false_type { };
template<typename T>
inline constexpr auto always_false_v = always_false<T>::value;

template<typename...>
using void_t = void;

template<typename, typename = void_t<> >
struct has_StateVariant_member : std::false_type { };
template<typename T>
struct has_StateVariant_member<T, void_t<typename T::StateVariant>> : std::true_type { };
template<typename T>
constexpr inline auto has_StateVariant_member_v = has_StateVariant_member<T>::value;

template<typename, typename = void_t<> >
struct has_EventVariant_member : std::false_type { };
template<typename T>
struct has_EventVariant_member<T, void_t<typename T::EventVariant>> : std::true_type { };
template<typename T>
constexpr inline auto has_EventVariant_member_v = has_EventVariant_member<T>::value;


template<typename... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<typename... Ts>
overloaded(Ts&&...) -> overloaded<Ts...>;

template<typename Variant, typename... Ts>
constexpr decltype(auto) match(Variant&& variant, Ts&&... ts) noexcept
{
    return std::visit(
        overloaded{ std::forward<Ts>(ts)... },
        std::forward<Variant>(variant)
        );
}


template<typename Derived, typename StateVariant, typename EventVariant>
class Fsm
{
    // static_assert(has_StateVariant_member_v<Derived>,
    //     "Fsm implementation class must define a StateVariant - std::variant of states");
    // static_assert(has_EventVariant_member_v<Derived>,
    //     "Fsm implementation class must define an EventVariant - std::variant of events");

    // using StateVariant = typename Derived::StateVariant;
    // using EventVariant = typename Derived::EventVariant;

public:
    constexpr void dispatch(const EventVariant& event)
    {
        auto fsm = derived();
        auto new_state = std::visit(
            [&fsm=fsm](auto&& e) noexcept -> std::optional<StateVariant>
                { return fsm.dispatch_event(std::forward<decltype(e)>(e)); }
        ,   event
        );
        if (new_state) {
            std::visit(
                [&fsm=fsm](auto&& s) noexcept { fsm.on_exit(std::forward<decltype(s)>(s)); },
                state_
                );
            state_ = *std::move(new_state);
            std::visit(
                [&fsm=fsm](auto&& s) noexcept { fsm.on_entry(std::forward<decltype(s)>(s)); },
                state_
            );
        }
    }

private:
    constexpr Derived& derived() noexcept
        { return static_cast<Derived&>(*this); }
    constexpr const Derived& derived() const noexcept
        { return static_cast<const Derived&>(*this); }

    constexpr auto& state() noexcept { return state_; }
    constexpr const auto& state() const noexcept { return state_; }

    // template<typename Event>
    constexpr auto dispatch_event(const EventVariant& event) -> std::optional<StateVariant>
    {
        return std::nullopt;
    }

    template<typename State>
    constexpr void on_entry(State&& s) { }

    template<typename State>
    constexpr void on_exit(State&& s) { }

    friend Derived;
    constexpr Fsm() noexcept = default;

    template<typename State>
        // TODO: enable_if State is one of the allowed variant types
    constexpr Fsm(State&& s) noexcept
        : state_{std::in_place_type_t<std::decay_t<State>>{}, std::forward<State>(s)}
        { }

    StateVariant state_{};
};

    struct StateIdle { };
    struct StatePaused
    {
        uint32_t counter_;
    };
    struct StateAnimating
    {
        uint32_t counter_;
    };

    struct EventUpdate { };
    struct EventPlay { };
    struct EventPause { };
    struct EventStop { };

using StateVariant = std::variant<StateAnimating, StatePaused, StateIdle>;
using EventVariant = std::variant<EventUpdate, EventPlay, EventPause, EventStop>;

class Animation : public Fsm<Animation, StateVariant, EventVariant>
{
public:
    friend class Fsm<Animation, StateVariant, EventVariant>;
    using base_type = Fsm<Animation, StateVariant, EventVariant>;
    using OptResult = std::optional<StateVariant>;

    constexpr Animation() noexcept
        : base_type{StateIdle{}}
        { }

    constexpr uint32_t current_count() const noexcept
    {
        return match(this->state(),
            [](const StateIdle& /*s*/) noexcept -> uint32_t { return 0; },
            [](const StateAnimating& s) noexcept -> uint32_t { return s.counter_; },
            [](const StatePaused& s) noexcept -> uint32_t { return s.counter_; },
            [](const auto& s) noexcept
                {static_assert(always_false_v<decltype(s)>, "Unhandled state"); }
            );
    }

    constexpr const char* current_state() const noexcept
    {
        return match(this->state(),
            [](const StateIdle&) noexcept { return "Idle"; },
            [](const StatePaused&) noexcept { return "StatePaused"; },
            [](const StateAnimating&) noexcept { return "StateAnimating"; },
            [](const auto& s) noexcept
                {static_assert(always_false_v<decltype(s)>, "Unhandled state"); }
            );
    }

private:
    constexpr auto dispatch_event(const EventVariant& e) -> std::optional<StateVariant>
    {
        return match(e,
            [this](const EventUpdate&){ return update(); },
            [this](const EventPlay&){ return play(); },
            [this](const EventPause&){ return pause(); },
            [this](const EventStop&){ return stop(); },
            [](const auto& ev){ static_assert(always_false_v<decltype(ev)>, "Unhandled event");}
        );
    }

    constexpr auto update() noexcept -> std::optional<StateVariant>
    {
        return std::visit(
            [](auto&& s) noexcept -> std::optional<StateVariant>
            {
                using T = std::decay_t<decltype(s)>;
                if constexpr (std::is_same_v<T, StateAnimating>) {
                    ++s.counter_;
                    if(s.counter_ >= counter_limit_) {
                        return StateIdle{};
                    }
                    return std::forward<decltype(s)>(s);
                }
                else {
                    return std::nullopt;
                }
            },
            this->state()
        );
    }

    constexpr auto play() noexcept -> std::optional<StateVariant>
    {
        return
            match(this->state(),
                [](const StateIdle&/*s*/) noexcept -> std::optional<StateVariant>
                    { return StateAnimating{0}; },
                [](const StatePaused& s) noexcept -> std::optional<StateVariant>
                    { return StateAnimating{s.counter_}; },
                [](const auto&) noexcept -> std::optional<StateVariant>
                    { return std::nullopt; }
                );
        // return std::visit(
        //     [this](auto&& s) noexcept -> std::optional<StateVariant>
        //     {
        //         using T = std::decay_t<decltype(s)>;
        //         if constexpr (std::is_same_v<T, StateIdle>) {
        //             return StateAnimating{0};
        //         }
        //         else if constexpr (std::is_same_v<T, StatePaused>) {
        //             return StateAnimating{s.counter_};
        //         }
        //         else {
        //             return std::nullopt;
        //         }
        //     },
        //     this->state()
        // );
    }

    constexpr auto stop() noexcept -> std::optional<StateVariant>
    {
        return std::visit(
            [](auto&& s) noexcept -> std::optional<StateVariant>
                {
                    using T = std::decay_t<decltype(s)>;
                    if constexpr (std::is_same_v<T, StateAnimating> ||
                                  std::is_same_v<T, StatePaused>) {
                        return StateIdle{};
                    }
                    else
                        return std::nullopt;
                },
            this->state()
        );
    }

    constexpr auto pause() noexcept -> std::optional<StateVariant>
    {
        return std::visit(
            [](auto&& s) noexcept -> std::optional<StateVariant>
            {
                using T = std::decay_t<decltype(s)>;
                if constexpr (std::is_same_v<T, StateAnimating>) {
                    return StatePaused{s.counter_};
                }
                else {
                    return std::nullopt;
                }
            },
            this->state()
        );
    }

// --- member data
    static constexpr inline auto counter_limit_ = 42u;
};


inline void run_animation(Animation& anim) noexcept
{
    anim.dispatch(EventPlay{});
    anim.dispatch(EventUpdate{});
    anim.dispatch(EventPause{});
    anim.dispatch(EventUpdate{});
    anim.dispatch(EventPlay{});
    anim.dispatch(EventUpdate{});
    anim.dispatch(EventStop{});
    anim.dispatch(EventPlay{});
    anim.dispatch(EventUpdate{});
    anim.dispatch(EventStop{});
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
        fsm.dispatch(EventPlay{});
        fsm.dispatch(EventUpdate{});
        std::cout << fsm.current_state() << ", count: " << fsm.current_count() << "\n";
        fsm.dispatch(EventPause{});
        fsm.dispatch(EventUpdate{});
        std::cout << fsm.current_state() << ", count: " << fsm.current_count() << "\n";
        fsm.dispatch(EventPlay{});
        fsm.dispatch(EventUpdate{});
        std::cout << fsm.current_state() << ", count: " << fsm.current_count() << "\n";
        fsm.dispatch(EventUpdate{});
        fsm.dispatch(EventUpdate{});
    }
    fsm.dispatch(EventStop{});
    fsm.dispatch(EventPause{});
    fsm.dispatch(EventUpdate{});
    std::cout << fsm.current_state() << ", count: " << fsm.current_count() << "\n";
}
