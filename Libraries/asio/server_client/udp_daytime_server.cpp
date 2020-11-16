#include <iostream>
#include <array>
#include <string>

#include <asio.hpp>


std::string make_daytime_string()
{
    std::time_t now{std::time(0)};
    return std::ctime(&now);
}


int main()
{
try {
    asio::io_context context;
    // create a socket
    asio::ip::udp::socket socket{context, asio::ip::udp::endpoint{asio::ip::udp::v4(), 13}};
    while (true)
    {
        // wait for a client to initiate contact
        std::array<char, 1> recv_buf;
        asio::ip::udp::endpoint remote_endpoint;
        socket.receive_from(asio::buffer(recv_buf), remote_endpoint);
        std::cout << "[UDP SERVER] Got message from remote " << remote_endpoint.address()
            << ", responding..." << std::endl;
        // respond to the client
        const std::string message = make_daytime_string();
        asio::error_code ec;
        socket.send_to(asio::buffer(message), remote_endpoint, 0, ec);
        if (ec) {
            std::cerr << "[UDP SERVER] Error: " << ec.message() << std::endl;
            break;
        }
    }
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