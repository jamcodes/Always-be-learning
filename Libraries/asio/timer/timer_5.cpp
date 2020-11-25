#include <iostream>
#include <chrono>
#include <thread>

#include <asio.hpp>


// Synchronising callback in a multithreaded program.
// Synchronisation is necessary only in prorgams which call io_context::run()
// from multiple threads and the handlers might be accessing a shared,
// thread-unsafe resource.

class Printer
{
public:
    Printer(asio::io_context& io)
        : strand_{asio::make_strand(io)},
          timer1_{io, std::chrono::seconds{1}},
          timer2_{io, std::chrono::seconds{1}}
        {
        // Note that the handler is now wrapped with asio::bind_executor that ties it to the strand.
        // This has to be done for every call to async_wait
            timer1_.async_wait(asio::bind_executor(strand_, [this](asio::error_code const& ec)noexcept{
                if (!ec)
                    print1();
                else
                    std::cout << "timer1 error: " << ec.message() << std::endl;
            }));
            timer2_.async_wait(asio::bind_executor(strand_, [this](asio::error_code const& ec)noexcept{
                if (!ec)
                    print2();
                else
                    std::cout << "timer2 error: " << ec.message() << std::endl;
            }));
        }

    ~Printer() noexcept
    {
        std::cout << "Final count is " << count_ << std::endl;
    }

    void print1()
    {
        if (count_ < 10)
        {
            std::cout << "Timer 1: " << count_ << std::endl;
            ++count_;
            timer1_.expires_at(timer1_.expiry() + std::chrono::seconds{1});
            timer1_.async_wait(asio::bind_executor(strand_, [this](asio::error_code const& ec)noexcept{
                if (!ec)
                    print1();
                else
                    std::cout << "timer1 error: " << ec.message() << std::endl;
            }));
        }
    }

    void print2()
    {
        if (count_ < 10)
        {
            std::cout << "Timer 2: " << count_ << std::endl;
            ++count_;
            timer2_.expires_at(timer2_.expiry() + std::chrono::seconds{1});
            timer2_.async_wait(asio::bind_executor(strand_,[this](asio::error_code const& ec)noexcept{
                if (!ec)
                    print2();
                else
                    std::cout << "timer2 error: " << ec.message() << std::endl;
            }));
        }
    }

private:
    asio::strand<asio::io_context::executor_type> strand_;
    asio::steady_timer timer1_;
    asio::steady_timer timer2_;
    int count_{0};
};


int main()
{
    // call the io_context object in two threads now
    asio::io_context io;
    Printer printer{io};
    std::thread t{[&io](){ io.run(); }};
    io.run();
    if (t.joinable()) t.join();
}
