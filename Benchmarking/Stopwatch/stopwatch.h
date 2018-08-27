#pragma once

#include <iostream>
#include <chrono>


template<typename T>
class basic_stopwatch : T
{
    using BaseTimer = T;
public:
    // create, optionally start timing an activity
    explicit basic_stopwatch(std::ostream& log, bool start=true) noexcept
        : log_{log}
        {
            if (start) {
                BaseTimer::start();
            }
        }
    explicit basic_stopwatch(const char* activity, bool start=true) noexcept
        : activity_{activity}
        {
            if (start) {
                BaseTimer::start();
            }
        }
    basic_stopwatch(std::ostream& log, const char* activity, bool start=true) noexcept
        : activity_{activity}, log_{log}
        {
            if (start) {
                BaseTimer::start();
            }
        }

    // stop and destroy the stopwatch
    ~basic_stopwatch() noexcept { stop(); }

    basic_stopwatch(const basic_stopwatch&) = delete;
    basic_stopwatch(basic_stopwatch&&) = delete;
    basic_stopwatch& operator=(const basic_stopwatch&) = delete;
    basic_stopwatch& operator=(basic_stopwatch&&) = delete;

    // get last lap time (time of last stop)
    unsigned lap_get() const noexcept { return lap_; }

    // predicate: return true if the stopwatch is running
    bool is_started() const noexcept { return BaseTimer::is_started(); }

    // show accumulated time, keep running, set/return lap
    unsigned show(const char* event="show")
    {
        lap_ = BaseTimer::get_ms();
        log_event(event);
        return lap_;
    }

    // (re)start a stopwatch, set/return lap time
    unsigned start(/* const char* event="start" */) noexcept
    {
        lap_ = BaseTimer::get_ms();
        // log_event(event);
        BaseTimer::start();
        return lap_;
    }

    // stop a running stopwatch, set/return lap time
    unsigned stop(/* const char* event="stop" */) noexcept
    {
        lap_ = BaseTimer::get_ms();
        BaseTimer::clear();
        // log_event(event);
        return lap_;
    }

private:
    inline void log_event(const char* const event) const noexcept
    {
        log_ << activity_ << ": " << event << " " << lap_ << "ms\n";
    }

    const char*   activity_{"Stopwatch"};
    unsigned      lap_{0};
    std::ostream& log_{std::cout};
};

class TimerBase
{
    using time_point = std::chrono::steady_clock::time_point;
    using clock_type = std::chrono::steady_clock;
    using milliseconds = std::chrono::milliseconds;
    using microseconds = std::chrono::microseconds;
public:
    TimerBase() noexcept
        : start_{time_point::min()}
        { }

    bool is_started() const noexcept
    {
        return start_.time_since_epoch() != clock_type::duration{0};
    }

    void clear() noexcept
    {
        start_ = time_point::min();
    }

    void start() noexcept
    {
        start_ = clock_type::now();
    }

    // get the number of milliseconds since the timer was started
    unsigned long get_ms() const noexcept
    {
        if (is_started()) {
            const auto diff = clock_type::now() - start_;
            return static_cast<unsigned long>(std::chrono::duration_cast<milliseconds>(diff).count());
        }
        return 0;
    }

private:
    time_point start_;
};

using Stopwatch = basic_stopwatch<TimerBase>;
