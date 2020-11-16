#include <iostream>
#include <array>

#include <asio.hpp>


int main(int argc, char* argv[])
{
try {
    if (argc != 2) {
        std::cerr << "Usage: client <host>" << std::endl;
        return 1;
    }

    asio::io_context context;
    // Same as with TCP, we need a resolver object, to find a remote endpoint to connect to
    // based on the host and service name.
    // Here the query is restricted do IPv4 addresses
    asio::ip::udp::resolver resolver{context};
    // udp::resolver::resolve() is guaranteed to return at least one endpoint
    // it's ok to dereference it directly
    asio::ip::udp::endpoint receiver_endpoint =
        *resolver.resolve(asio::ip::udp::v4(), argv[1], "daytime").begin();

    // UDP is datagram oriented, so a naked socket is used, instead of a stream socket
    asio::ip::udp::socket socket{context};
    // open the socket
    socket.open(asio::ip::udp::v4());

    // send data to the endpoint
    std::array<char, 1> send_buf{0};
    socket.send_to(asio::buffer(send_buf), receiver_endpoint);

    // prepare to receive data from the endpoint
    std::array<char, 128> recv_buf;
    asio::ip::udp::endpoint sender_endpoint;
    const std::size_t len = socket.receive_from(asio::buffer(recv_buf), sender_endpoint);
    std::cout.write(recv_buf.data(), static_cast<std::streamsize>(len));
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