#include <array>
#include <iostream>
#include <future>
#include <thread>

#include <asio.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/udp.hpp>
#include <asio/use_future.hpp>


void get_daytime(asio::io_context& context, const char* hostname)
{
try
{
    asio::ip::udp::resolver resolver{context};
    // The .async_resolve() call given asio::use_future, returns the endpoints
    // as a future value that is not retrieved
    std::future<asio::ip::udp::resolver::results_type> endpoints{
        resolver.async_resolve(asio::ip::udp::v4(), hostname, "daytime", asio::use_future)
    };

    asio::ip::udp::socket socket{context, asio::ip::udp::v4()};

    const std::array<char, 1> send_buf{0};
    std::future<std::size_t> send_length{
        socket.async_send_to(asio::buffer(send_buf),
            *endpoints.get().begin(),     // the endpoints future object asked for a result here - may block
            asio::use_future)
    };

    // other tasks may be done here - while the send completes

    send_length.wait();  // blocks until the send is complete. May throw

    std::array<char, 128> recv_buf;
    asio::ip::udp::endpoint sender_endpoint;
    std::future<std::size_t> recv_length{
        socket.async_receive_from(asio::buffer(recv_buf), sender_endpoint, asio::use_future)
    };

    // other tasks may be done here while the receive completes

    std::cout.write(recv_buf.data(), static_cast<std::streamsize>(recv_length.get()));
}
catch (std::system_error const& e) {
    std::cerr << e.what() << std::endl;
}
}


int main(int argc, char* argv[])
{
try {
    if (argc != 2) {
        std::cerr << "Usage: daytime_client <host>" << std::endl;
        return 1;
    }

    // Run the io_context on a dedicated thread, so that it operates completely
    // asynchronously to the rest of the program
    asio::io_context context;
    auto work{asio::require(context.get_executor(), asio::execution::outstanding_work.tracked)};
    std::thread thread{[&context](){context.run();}};

    get_daytime(context, argv[1]);

    context.stop();
    if (thread.joinable()) thread.join();
}
catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
    return 2;
}
}
