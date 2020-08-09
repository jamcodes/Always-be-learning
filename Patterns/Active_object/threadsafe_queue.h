#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <type_traits>
#include <utility>

/**
 * An alternative implementation of a single-mutex threadsafe queue.
 * Stack data is allocated at the call to push() - bringing allocation outside of the region
 * protected by the mutex - thus increasing opportunity for concurrency.
 */

template <typename T>
class threadsafe_queue {
  public:
    using value_type = T;
    threadsafe_queue() = default;

    threadsafe_queue(threadsafe_queue const& other)
        : queue_{std::lock_guard<std::mutex>{other.mtx_}, other.queue_}
    {
    }

    threadsafe_queue(threadsafe_queue&& other) noexcept(
        std::is_nothrow_move_constructible_v<std::queue<value_type>>)
        : queue_{(std::lock_guard<std::mutex>{other.mtx_}, std::move(other.queue_))}
    {
    }

    threadsafe_queue& operator=(threadsafe_queue const& other)
    {
        if (this == &other) {
            return *this;
        }
        {
            std::lock(mtx_, other.mtx_);
            std::lock_guard<std::mutex> lk_this{mtx_, std::adopt_lock};
            std::lock_guard<std::mutex> lk_other{other.mtx_, std::adopt_lock};
            queue_ = other.queue_;
        }
        return *this;
    }

    threadsafe_queue& operator=(threadsafe_queue&& other) noexcept(
        std::is_move_assignable_v<std::queue<value_type>>)
    {
        if (this == &other) {
            return *this;
        }
        {
            std::lock(mtx_, other.mtx_);
            std::lock_guard<std::mutex> lk_this{mtx_, std::adopt_lock};
            std::lock_guard<std::mutex> lk_other{other.mtx_, std::adopt_lock};
            queue_ = other.queue_;
        }
        return *this;
    }

    void push(T value)
    {
        std::lock_guard<std::mutex> lock{mtx_};
        queue_.push(std::move(value));
        cv_.notify_one();
    }

    template <typename... Args>
    std::enable_if_t<std::is_constructible_v<T, Args...>>
    emplace(Args&&... args)
    {
        std::lock_guard<std::mutex> lock{mtx_};
        queue_.emplace(std::forward<Args>(args)...);
        cv_.notify_one();
    }

    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lock{mtx_};
        cv_.wait(lock, [this] { return !queue_.empty(); });
        value = std::move(queue_.front());
        queue_.pop();
        lock.unlock();
    }

    bool try_pop(T& value)
    {
        std::unique_lock<std::mutex> lock{mtx_};
        if (queue_.empty()) {
            return false;
        }
        value = std::move(queue_.front());
        queue_.pop();
        lock.unlock();
        return true;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock{mtx_};
        return queue_.empty();
    }

  private:
    mutable std::mutex mtx_{};
    std::queue<value_type> queue_{};
    std::condition_variable cv_{};
};
