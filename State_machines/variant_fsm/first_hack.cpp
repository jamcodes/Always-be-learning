#include <iostream>
#include <string>
#include <variant>
#include <type_traits>
#include <chrono>
#include <Stopwatch/stopwatch.h>

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


class Animation
{
public:
    using State = std::variant<StateIdle, StatePaused, StateAnimating>;

    constexpr Animation() noexcept = default;

    constexpr uint32_t current_count() const noexcept
    {
        return std::visit([](auto&& state) noexcept -> uint32_t
        {
            using T = std::decay_t<decltype(state)>;
            if constexpr (std::is_same_v<T, StateIdle>) {
                return 0;
            }
            else if constexpr (std::is_same_v<T, StateAnimating> ||
                std::is_same_v<T, StatePaused>) {
                return state.counter_;
            }
            else {
                static_assert(always_false_v<T>, "Non-exhaustive visitor!");
            }
        }, state_);
    }

    constexpr const char* current_state() const noexcept
    {
        return std::visit([](auto&& state) noexcept -> const char*
        {
            using T = std::decay_t<decltype(state)>;
            if constexpr (std::is_same_v<T, StateIdle>) {
                return "Idle";
            } else if constexpr (std::is_same_v<T, StateAnimating>) {
                return "Animating";
            } else if constexpr (std::is_same_v<T, StatePaused>) {
                return "Paused";
            } else {
                static_assert(always_false_v<T>, "Non-exhaustive visitor!");
            }
        }, state_);
    }

    constexpr void update() noexcept
    {
        std::visit([this](auto&& state) noexcept
        {
            using T = std::decay_t<decltype(state)>;
            if constexpr (std::is_same_v<T, StateAnimating>) {
                ++state.counter_;
                if (state.counter_ >= counter_limit) {
                    state_ = StateIdle{};
                }
            }
        }, state_);
    }

    constexpr void play() noexcept
    {
        std::visit([this](auto&& state) noexcept
        {
            using T = std::decay_t<decltype(state)>;
            if constexpr (std::is_same_v<T, StateIdle>) {
                state_ = StateAnimating{0};
            } else if constexpr (std::is_same_v<T, StatePaused>) {
                state_ = StateAnimating{state.counter_};
            }
        }, state_);
    }

    constexpr void stop() noexcept
    {
        std::visit([this](auto&& state)
        {
            using T = std::decay_t<decltype(state)>;
            if constexpr (std::is_same_v<T, StateAnimating> || std::is_same_v<T, StatePaused>) {
                state_ = StateIdle{};
            }
        }, state_);
    }

    constexpr void pause() noexcept
    {
        std::visit([this](auto&& state) noexcept
        {
            using T = std::decay_t<decltype(state)>;
            if constexpr (std::is_same_v<T, StateAnimating>) {
                state_ = StatePaused{state.counter_};
            }
        }, state_);
    }

private:
    static constexpr inline uint32_t counter_limit{144};
    State state_{StateIdle{}};
};


inline void run_animation(Animation& anim) noexcept
{
    anim.play();
    anim.update();
    anim.pause();
    anim.update();
    anim.play();
    anim.update();
    anim.stop();
    anim.play();
    anim.update();
    anim.stop();
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
        fsm.play();
        fsm.update();
        std::cout << fsm.current_state() << ", count: " << fsm.current_count() << "\n";
        fsm.pause();
        fsm.update();
        std::cout << fsm.current_state() << ", count: " << fsm.current_count() << "\n";
        fsm.play();
        fsm.update();
        std::cout << fsm.current_state() << ", count: " << fsm.current_count() << "\n";
        fsm.update();
        fsm.update();
    }
    fsm.stop();
    fsm.pause();
    fsm.update();
    std::cout << fsm.current_state() << ", count: " << fsm.current_count() << "\n";
}
