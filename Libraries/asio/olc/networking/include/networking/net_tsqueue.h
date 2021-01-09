#pragma once

#include "net_common.h"

#include <condition_variable>


namespace olc
{
namespace net
{

template<typename T>
class tsqueue {
public:
    tsqueue() = default;
    tsqueue& operator=(tsqueue&&) = delete;

    ~tsqueue() noexcept
    {
        clear();
    }

    T pop_front()
    {
        // expects !data_.empty();
        std::lock_guard lk{mutex_};
        auto val{std::move(data_.front())};
        data_.pop_front();
        return val;
    }

    T pop_back()
    {
        // expects !data_.empty();
        std::lock_guard lk{mutex_};
        auto val{std::move(data_.back())};
        data_.pop_back();
        return val;
    }

    T wait_pop_front()
    {
        // std::unique_lock<std::mutex> lk{mutex_};
        // cv_.wait(lk, [this]()noexcept{ return !data_.empty(); });
        auto lk{do_wait_for_data()};
        auto val{std::move(data_.front())};
        data_.pop_front();
        return val;
    }

    T wait_pop_back()
    {
        // std::unique_lock<std::mutex> lk{mutex_};
        // cv_.wait(lk, [this]()noexcept{ return !data_.empty(); });
        auto lk{do_wait_for_data()};
        auto val{std::move(data_.back())};
        data_.pop_back();
        return val;
    }

    void push_front(T const& val)
    {
        std::lock_guard lk{mutex_};
        data_.push_front(val);
        cv_.notify_one();
    }

    void push_front(T&& val)
    {
        std::lock_guard lk{mutex_};
        data_.push_front(std::move(val));
        cv_.notify_one();
    }

    void push_back(T const& val)
    {
        std::lock_guard lk{mutex_};
        data_.push_back(val);
        cv_.notify_one();
    }


    void push_back(T&& val)
    {
        std::lock_guard lk{mutex_};
        data_.push_back(std::move(val));
        cv_.notify_one();
    }

    std::size_t count() const noexcept
    {
        std::lock_guard lk{mutex_};
        return data_.size();
    }

    bool empty() const noexcept
    {
        std::lock_guard lk{mutex_};
        return data_.empty();
    }

    void clear()
    {
        std::lock_guard lk{mutex_};
        data_.clear();
    }

    void wait_for_data()
    {
        (void)do_wait_for_data();
    }

private:
    std::unique_lock<std::mutex> do_wait_for_data()
    {
        std::unique_lock<std::mutex> lk{mutex_};
        cv_.wait(lk, [this]()noexcept{ return !data_.empty(); });
        return lk;
    }

    mutable std::mutex mutex_{};
    mutable std::condition_variable cv_{};
    std::deque<T> data_{};
};

} // namespace net
} // namespace olc

