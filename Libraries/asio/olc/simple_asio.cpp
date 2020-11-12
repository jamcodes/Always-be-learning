#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <utility>
#include <thread>
#include <type_traits>

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <fmt/core.h>

using namespace std::literals::string_literals;
using namespace std::literals::chrono_literals;

std::vector<char> buffer(20 * 1024);

void get_some_data(asio::ip::tcp::socket& socket)
{
    socket.async_read_some(asio::buffer(buffer.data(), buffer.size()),
        [&socket](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                fmt::print("\n\nRead {} bytes\n\n", length);
                // auto it{buffer.cbegin()};
                // const auto end{std::next(it, static_cast<std::ptrdiff_t>(length))};
                // for (; it != end; ++it) {
                //     fmt::print("{}", *it);
                // }
                for (std::size_t i{0}; i != length; ++i) {
                    std::cout << buffer[i];
                }
                get_some_data(socket);
            }
        }
    );
}


int main()
{
    fmt::print("Hello asio world!\n");
    asio::error_code ec;

    // get the context - abstraction over platform-specific handle
    asio::io_context context;
    // give the context some fake/idle tasks to do, so that it doesn't finish right away
    asio::io_context::work idle_work{context};
    // run the context on a dedicated thread
    std::thread context_thread{[&context](){ context.run(); }};

    // endpoint - address of somewhere to connect to
    asio::ip::tcp::endpoint endpoint{asio::ip::make_address("51.38.81.49", ec), 80};
    // create a socket - the context will deliver the implementation
    asio::ip::tcp::socket socket{context};

    socket.connect(endpoint, ec);

    if (!ec)
    {
        fmt::print("Connected!\n");
    }
    else
    {
        fmt::print("Failed to connect to address: {}\n", ec.message());
    }

    if (socket.is_open())
    {
        // prime the context with instructions on what to do when data becomes available
        get_some_data(socket);
        
        std::string request("GET /index.html HTTP/1.1\r\n"
                             "Host: example.com\r\n"
                             "Connection: close\r\n\r\n");

        socket.write_some(asio::buffer(request.data(), request.size()), ec);

        std::this_thread::sleep_for(20s);

        // instruct the context to stop doing work - necessary since we've given it idle-tasks to do
        context.stop();
        if (context_thread.joinable()) context_thread.join();

        // if reading communicating synchronously - we'd need to wait for the data
        // socket.wait(socket.wait_read);

        // std::size_t bytes{socket.available()};
        // fmt::print("Bytes available: {}\n", bytes);

        // if (bytes > 0)
        // {
        //     std::vector<char> buffer(bytes);
        //     socket.read_some(asio::buffer(buffer.data(), buffer.size()), ec);
        //     for (auto c : buffer){
        //         fmt::print("{}", c);
        //     }
        // }
    }

    fmt::print("Press enter to continue...\n");
    (void)std::cin.get();
}
