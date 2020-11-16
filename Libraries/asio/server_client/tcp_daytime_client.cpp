#include <array>
#include <iostream>

#include <asio.hpp>


int main(int argc, char* argv[])
{
try{
    if (argc != 2) {
        std::cerr << "Usage: client <host>" << std::endl;
        return 1;
    }

    // all asio programs need at least one io_context
    asio::io_context context;

    // specified server name needs to be resolved into a TCP endpoint
    asio::ip::tcp::resolver resolver{context};
    // a resolver takes a host name and a service name and turns them into
    // a list of endpoints - it may contain both IPv4 and IPv6 endpoints
    // standard port for the "Daytime" service is port 13
    asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "daytime");

    asio::ip::tcp::socket socket{context};
    // asio::connect will handle selecting an endpoint that works out of
    // the endpoints returned by the resolver
    asio::connect(socket, endpoints);

    while (true) {
        std::array<char, 128> buf;
        asio::error_code ec;

        const std::size_t len = socket.read_some(asio::buffer(buf), ec);

        if (ec == asio::error::eof) {
            break;  // connection closed cleanly by peer
        }
        else if (ec) {
            throw asio::system_error{ec};   // some error occured
        }

        std::cout.write(buf.data(), static_cast<std::streamsize>(len));
    };

}
catch (std::exception const& e)
{
    std::cerr << e.what() << std::endl;
}
catch (...)
{
    std::cerr << "Unknown error" << std::endl;
}
}