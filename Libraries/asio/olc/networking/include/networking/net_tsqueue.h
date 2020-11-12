#pragma once

#include "net_common.h"


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
        std::lock_guard lk{mutex_};
        auto val{std::move(data_.front())};
        data_.pop_front();
        return val;
    }

    T pop_back()
    {
        std::lock_guard lk{mutex_};
        auto val{std::move(data_.back())};
        data_.pop_back();
        return val;
    }

    void push_front(T const& val)
    {
        std::lock_guard lk{mutex_};
        data_.push_front(val);
    }

    void push_front(T&& val)
    {
        std::lock_guard lk{mutex_};
        data_.push_front(std::move(val));
    }

    void push_back(T const& val)
    {
        std::lock_guard lk{mutex_};
        data_.push_back(val);
    }


    void push_back(T&& val)
    {
        std::lock_guard lk{mutex_};
        data_.push_back(std::move(val));
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
    
private:
    mutable std::mutex mutex_{};
    std::deque<T> data_{};
};

} // namespace net
} // namespace olc

