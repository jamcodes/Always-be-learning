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


template<typename Derived, typename... States>
class Fsm
{
    // static_assert(std::is_same_v<std::monostate, std::variant_alternative_t<0, StateVariant>>,
    //               "FSM: StateVariant type must hold std::monostate at index 0");
public:
    using StateVariant = std::variant<std::monostate, States...>;

    template<typename Event>
        // TODO: enable_if EventVariant can be assigned Event type
    /* constexpr */ void dispatch(Event&& event)
    {
        auto fsm = derived();
        auto new_state = fsm.dispatch_event(std::forward<Event>(event));
        if (!std::holds_alternative<std::monostate>(new_state)) {
            std::visit(
                [&fsm=fsm](auto&& s) noexcept { fsm.on_exit(std::forward<decltype(s)>(s)); },
                state_
            );

            state_ = std::move(new_state);

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

    template<typename Event>
    constexpr auto dispatch_event(Event&& /*e*/) -> StateVariant
    {
        return std::monostate{};
    }

    template<typename State>
        // TODO: assert that State is a StateVariant type
    constexpr void on_entry(State&& s) { }

    template<typename State>
        // TODO: assert that State is a StateVariant type
    constexpr void on_exit(State&& s) { }

    friend Derived;
    constexpr Fsm() noexcept = default;

    template<typename State>
        // TODO: enable_if State is one of the allowed variant types
    constexpr Fsm(State&& s) noexcept
        : state_{std::in_place_type_t<std::decay_t<State>>{}, std::forward<State>(s)}
        { }

// --- member data
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


class Animation : public Fsm<Animation, StateAnimating, StatePaused, StateIdle>
{
public:
    friend class Fsm;
    using base_type = Fsm<Animation, StateAnimating, StatePaused, StateIdle>;
    using StateVariant = typename base_type::StateVariant;

    constexpr Animation() noexcept
        : base_type{StateIdle{}}
        { }

    constexpr uint32_t current_count() const noexcept
    {
        return match(this->state(),
            [](const StateIdle& /*s*/) noexcept -> uint32_t { return 0; },
            [](const StateAnimating& s) noexcept -> uint32_t { return s.counter_; },
            [](const StatePaused& s) noexcept -> uint32_t { return s.counter_; },
            [](std::monostate) noexcept { return 144u; }
            // [](const auto& s) noexcept
            //     {static_assert(always_false_v<decltype(s)>, "Unhandled state"); }
            );
    }

    constexpr const char* current_state() const noexcept
    {
        return match(this->state(),
            [](const StateIdle&) noexcept { return "Idle"; },
            [](const StatePaused&) noexcept { return "StatePaused"; },
            [](const StateAnimating&) noexcept { return "StateAnimating"; },
            [](std::monostate) noexcept { return "monostate"; }
            // [](const auto& s) noexcept
            //     {static_assert(always_false_v<decltype(s)>, "Unhandled state"); }
            );
    }

private:
    template<typename Event>
    constexpr auto dispatch_event(Event&& /*e*/) -> StateVariant
    {
        using T = std::decay_t<Event>;
        if constexpr (std::is_same_v<T, EventUpdate>) {
            return update();
        }
        else if constexpr (std::is_same_v<T, EventPlay>) {
            return play();
        }
        else if constexpr (std::is_same_v<T, EventPause>) {
            return pause();
        }
        else if constexpr (std::is_same_v<T, EventStop>) {
            return stop();
        }
        else {
            static_assert(always_false_v<T>, "Unhandled event type");
        }
    }

    constexpr auto update() noexcept -> StateVariant
    {
        return std::visit(
            [](auto&& s) noexcept -> StateVariant
            {
                using T = std::decay_t<decltype(s)>;
                if constexpr (std::is_same_v<T, StateAnimating>) {
                    ++s.counter_;
                    if(s.counter_ >= counter_limit_) {
                        return StateIdle{};
                    }
                    // return std::monostate{};
                    // must return an object here, even though we don't want to traisiton,
                    // otherwise the changes have no effect...
                    return std::forward<decltype(s)>(s);
                }
                else {
                    return std::monostate{};
                }
            },
            this->state()
        );
    }

    constexpr auto play() noexcept -> StateVariant
    {
        return
            match(this->state(),
                [](const StateIdle&/*s*/) noexcept -> StateVariant
                    { return StateAnimating{0}; },
                [](const StatePaused& s) noexcept -> StateVariant
                    { return StateAnimating{s.counter_}; },
                [](const auto&) noexcept -> StateVariant
                    { return std::monostate{}; }
                );
    }

    constexpr auto stop() noexcept -> StateVariant
    {
        return std::visit(
            [](auto&& s) noexcept -> StateVariant
                {
                    using T = std::decay_t<decltype(s)>;
                    if constexpr (std::is_same_v<T, StateAnimating> ||
                                  std::is_same_v<T, StatePaused>) {
                        return StateIdle{};
                    }
                    else
                        return std::monostate{};
                },
            this->state()
        );
    }

    constexpr auto pause() noexcept -> StateVariant
    {
        return std::visit(
            [](auto&& s) noexcept -> StateVariant
            {
                using T = std::decay_t<decltype(s)>;
                if constexpr (std::is_same_v<T, StateAnimating>) {
                    return StatePaused{s.counter_};
                }
                else {
                    return std::monostate{};
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
