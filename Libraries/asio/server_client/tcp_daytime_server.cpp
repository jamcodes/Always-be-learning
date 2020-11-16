#include <ctime>
#include <chrono>
#include <iostream>
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

    // servers listen for new connections using a tcp::acceptor
    // A "daytime" standard port is port 13
    asio::ip::tcp::acceptor acceptor{context, asio::ip::tcp::endpoint{asio::ip::tcp::v4(), 13}};

    // this server handles one connection at a time
    for (;;)
    {
        asio::ip::tcp::socket socket{context};
        acceptor.accept(socket);
        const std::string message{make_daytime_string()};

        asio::error_code ignored_error;
        asio::write(socket, asio::buffer(message), ignored_error);
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
