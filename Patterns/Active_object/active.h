#pragma once

#include <thread>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <utility>

#include "threadsafe_queue.h"


namespace jam
{

// Active Object are a design pattern that encapsulates asynchronous workloads, by managing
// a thread and a message-queue of tasks that the thread executes. That way the caller can
// asynchronously post tasks to the Active Object's queue without the need for blocking.
//
// Active Objects are generally usefuly wherever a thread would be - to express long running
// services - physics thread, GUI thread, etc., to decouple independent work - background save,
// pipeline stages...

class Active {
public:
    using Callback = std::function<void()>;


    Active(Active&&) noexcept = default;
    Active(Active const&) = default;
    ~Active();

    static Active create()
    {
        Active a;
        a.thread_ = std::thread{[&a](){ a.run(); }};
        return a;
    }

    void send(Callback msg);


private:
    Active() = default;

    void do_done() noexcept { done_ = true; }
    void run();

    threadsafe_queue<Callback> queue_{};
    std::thread thread_{};
    bool done_{false};     // to be set only on the active thread via a callback

};

// Active object base class
// to be inherited from. Active object instances created using a factory member function
class Active2 {
public:
    using Callback = std::function<void()>;

    void send(Callback msg);

    template<typename T, typename... Args>
    static T create(Args&&... args)
    {
        T active{std::forward<Args>(args)...};
        // do_create(static_cast<jam::Active2&>(active));
        static_cast<Active2&>(active).thread_ = std::thread{[&active](){ active.run(); }};
        return active;
    }

protected:
    Active2() = default;
    Active2(Active2 const&) = default;
    Active2(Active2&&) noexcept = default;
    Active2& operator=(Active2 const&) = default;
    Active2& operator=(Active2&&) noexcept = default;
    ~Active2() noexcept;

private:
    void run();
    void do_done() noexcept { done_ = true; }
    // void init_thread(std::thread& t);
    // static void do_create(Active2& a);

    threadsafe_queue<Callback> queue_{};
    std::thread thread_{};
    bool done_{false};
};


// Active object base class using CRTP.
// Make use'age safer and allow for non-public inheritance. Creation is done using
// a non-templated factory member function
template<typename Derived>
class Active3 {
public:
    using Callback = std::function<void()>;

    void send(Callback msg)
    {
        queue_.push(std::move(msg));
    }

    template<typename... Args>
    static Derived create(Args&&... args)
    {
        Derived active{std::forward<Args>(args)...};
        active.thread_ = std::thread{[&active](){ active.run(); }};
        return active;
    }

protected:
    friend Derived;
    Active3() = default;
    Active3(Active3 const&) = default;
    Active3(Active3&&) = default;
    Active3& operator=(Active3 const&) = default;
    Active3& operator=(Active3&&) = default;
    ~Active3() noexcept
    {
        send([this](){ do_done(); });
        thread_.join();
    }

private:
    Derived& derived() & noexcept { return static_cast<Derived&>(*this); }
    Derived const& derived() const& noexcept { return static_cast<Derived const&>(*this); }
    Derived&& derived() && noexcept { return static_cast<Derived&&>(*this); }
    Derived const&& derived() const&& noexcept { return static_cast<Derived const&&>(*this); }

    void run()
    {
        while (!done_) {
            // wait until a task is available, then pop it and execute it
            Callback cb;
            queue_.wait_and_pop(cb);
            cb();
        }
    }

    void do_done() noexcept { done_ = true; }

    threadsafe_queue<Callback> queue_{};
    std::thread thread_{};
    bool done_{false};
};

} // namespace jam
