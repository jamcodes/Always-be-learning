#include <iostream>
#include <chrono>

#include <asio.hpp>
#include <fmt/core.h>


// Implement a handler function with the desired signature.
// It will register another async callback provided that some conditions are met
void print(asio::error_code const& ec, asio::steady_timer& timer, int& count)
{
    if (!ec)
    {
        if (count < 5) {
            // fmt::print(stderr, "count: {}\n", count);
            std::cerr << "count: " << count << std::endl;
            ++count;
            // extend the expiry timer
            timer.expires_at(timer.expiry() + std::chrono::seconds{1});
            // and register the callback again
            timer.async_wait([&timer, &count](asio::error_code const& e){
                print(e, timer, count);
            });
        }
    }
    else
    {
        // fmt::print(stderr, "steady_timer error: {}\n", ec.message());
    }
}

int main()
{
    asio::io_context io;
    asio::steady_timer timer{io, std::chrono::seconds{1}};
    int count{0};
    // register the callback - the expected signature takes asio::error_code as the sole parameter,
    // so the remaining parameters must be bound in lambda captures.
    timer.async_wait([&timer, &count](asio::error_code const& ec){
        print(ec, timer, count);
    });
    // start the io_context
    io.run();
    // show that `count` was being used
    // fmt::print("Final count is {}", count);
    std::cerr << "Final count is: " << count << std::endl;
}
