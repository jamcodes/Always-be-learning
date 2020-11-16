#include <chrono>

#include <asio.hpp>
#include <fmt/core.h>


int main()
{
    // all asio programs need at least one io_context
    asio::io_context io;

    // all core asio classes take a reference to io_context as their first ctor argument
    asio::steady_timer timer{io, std::chrono::seconds(5)};

    // blocking call
    // wait until the time specified in at construction expires
    // - counting from when the `timer` was constructed, not when .wait() was called!
    timer.wait();
    fmt::print("Hello World!\n");
}
