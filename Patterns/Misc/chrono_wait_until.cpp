#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>
#include <random>
#include <utility>


template<typename Engine = std::default_random_engine,
         typename Distribution = std::uniform_int_distribution<>
        >
class Rng {
public:
    using random_engine_type = Engine;
    using distribution_type = Distribution;
    using result_type = typename distribution_type::result_type;

    Rng() = default;
    Rng(result_type min, result_type max)
        : ud_{min, max}
    {
    }

    result_type operator()() noexcept { return ud_(re_); }

private:
    std::default_random_engine re_{
        std::chrono::steady_clock::now().time_since_epoch().count()
    };
    std::uniform_int_distribution<> ud_{500, 2000};
};


class Waiter {
public:
    Waiter() noexcept = default;

    template<typename Clock>
    void operator()(std::chrono::time_point<Clock> timeout)
    {
        std::unique_lock<std::mutex> lock{mtx_};
        while (cv_.wait_until(lock, timeout) != std::cv_status::timeout)
            ;
        std::cerr << "waiter " << reinterpret_cast<void const*>(this) << " done\n";
    }

private:
    static inline std::condition_variable cv_{};
    static inline std::mutex mtx_{};
};

int main()
{
    std::condition_variable cv;
    std::mutex m;

    auto t1 = std::thread([&cv=cv, &m=m]() mutable noexcept{
        auto const timeout = std::chrono::steady_clock::now() +
            std::chrono::seconds{2};
        std::unique_lock<std::mutex> lock{m};
        while (true) {
            if (cv.wait_until(lock, timeout) == std::cv_status::timeout) {
                break;
            }
        }
        std::cerr << "Thread1 done\n";
        // p.set_value();
    });

    auto t2 = std::thread([&cv=cv, &m=m]() mutable noexcept{
        auto const timeout = std::chrono::steady_clock::now() +
            std::chrono::seconds{1};
        std::unique_lock<std::mutex> lock{m};

        while (cv.wait_until(lock, timeout) != std::cv_status::timeout) {
        }
        std::cerr << "Thread2 done\n";
        // p.set_value();
    });

    Rng rng{};
    auto t3 = std::thread(Waiter{},
                         std::chrono::steady_clock::now()+std::chrono::milliseconds{rng()});
    auto t4 = std::thread(Waiter{},
                        std::chrono::steady_clock::now()+std::chrono::milliseconds{rng()});

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
