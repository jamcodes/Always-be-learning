#include <iostream>
#include <string>


enum class State
{
    Animating,
    Paused,
    Idle
};

class Animation
{
public:
    constexpr Animation() noexcept = default;

    constexpr uint32_t current_count() const noexcept
    {
        switch (state_)
        {
        case State::Animating:
        case State::Paused:
            return counter_;
        case State::Idle:
        default:
            return 0u;
        }
    }

    constexpr const char* current_state() const noexcept
    {
        switch (state_)
        {
        case State::Animating:
            return "Animating";
        case State::Paused:
            return "Paused";
        case State::Idle:
            return "Idle";
        default:
            return "Unknown";
        }
    }

    constexpr void update() noexcept
    {
        switch (state_)
        {
        case State::Animating:
            animate();
            break;
        case State::Paused:
        case State::Idle:
        default:
            break;
        }
    }

    constexpr void play() noexcept
    {
        state_ = State::Animating;
    }

    constexpr void stop() noexcept
    {
        counter_ = 0;
        state_ = State::Idle;
    }

    constexpr void pause() noexcept
    {
        if (state_ == State::Animating) {
            state_ = State::Paused;
        }
    }

protected:
    constexpr void animate() noexcept
    {
        ++counter_;
        if (counter_ >= counter_limit) {
            stop();
        }
    }

private:
    State state_{State::Idle};
    uint32_t counter_{0};

    static constexpr inline uint32_t counter_limit{144};
};


int main()
{
    auto fsm = Animation{};
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
