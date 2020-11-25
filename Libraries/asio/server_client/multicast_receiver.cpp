#include <array>
#include <iostream>
#include <string>

#include <asio.hpp>

namespace
{
constexpr short Multicast_Port{30001};
} // namespace


class Receiver
{
public:
    Receiver(asio::io_context& context,
             asio::ip::address const& listen_address,
             asio::ip::address const& multicast_address)
        : socket_{context}
    {
        // initialize the socket so that multiple addresses may be bound
        // to the same address
        asio::ip::udp::endpoint listen_endpoint{listen_address, Multicast_Port};
        socket_.open(listen_endpoint.protocol());
        socket_.set_option(asio::ip::udp::socket::reuse_address(true));
        socket_.bind(listen_endpoint);

        // Join the multicast group
        socket_.set_option(asio::ip::multicast::join_group(multicast_address));

        do_receive();
    }

private:
    void do_receive()
    {
        socket_.async_receive_from(asio::buffer(data_), sender_endpoint_,
            [this](std::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    std::cout.write(data_.data(), static_cast<std::streamsize>(length));
                    std::cout << std::endl;

                    do_receive();
                }
                else
                {
                    std::cerr << "[Multicast Receiver] Error: " << ec.message() << std::endl;
                }
            }
        );
    }

    asio::ip::udp::socket socket_;
    asio::ip::udp::endpoint sender_endpoint_{};
    std::array<char, 1024> data_{};
};


int main(int argc, char* argv[])
{
try {
    if (argc != 3)
    {
        std::cerr << "Usage: multicast_receiver <listen_address> <multicast_address>\n"
                  << "  For IPv4 try:\n"
                  << "    receiver 0.0.0.0 239.255.0.1\n"
                  << "  For IPv6 try:\n"
                  << "    receiver 0::0 ff31::8000:1234\n";
        return 1;
    }

    asio::io_context context;
    Receiver r{context, asio::ip::make_address(argv[1]), asio::ip::make_address(argv[2])};
    context.run();
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