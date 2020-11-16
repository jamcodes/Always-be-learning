#include <chrono>

#include <asio.hpp>
#include <fmt/core.h>


int main()
{
    asio::io_context io;
    asio::steady_timer timer{io, std::chrono::seconds{5}};

    // asynchronous call
    // give the timer a callback to call when the timer expires
    timer.async_wait([](asio::error_code const& ec){
        if (!ec)
            fmt::print("Hello async World!\n");
        else
            fmt::print("steady_timer error: {}", ec.message());
    });

    // asio guarantees that callback handlers will be called only from the thread calling
    // io_context::run() - if run() wasn't called the callback would never be called either.
    // The io_context must be given some work to do before run() is called - run() will block
    // only untill it has some work to do - here work is waiting on the timer to expire.
    io.run();
}
