#include <iostream>
#include <sstream>
#include <string>

#include <asio.hpp>
#include <fmt/core.h>

namespace
{
constexpr short Multicast_Port{30001};
constexpr int Max_Message_Count{10};
} // namespace


class Sender
{
public:
    Sender(asio::io_context& context,
           asio::ip::address const& multicast_address)
        : endpoint_{multicast_address, Multicast_Port},
          socket_{context, endpoint_.protocol()},
          timer_{context}
        {
            do_send();
        }

private:
    void do_send()
    {
        ++message_count_;
        message_ = fmt::format("Message {}", message_count_);

        socket_.async_send_to(asio::buffer(message_), endpoint_,
            [this](std::error_code ec, std::size_t /* length */)
            {
                if (!ec && message_count_ < Max_Message_Count)
                {
                    do_timeout();
                }
            }
        );
    }

    void do_timeout()
    {
        timer_.expires_after(std::chrono::seconds{1});
        timer_.async_wait(
            [this](std::error_code ec)
            {
                if (!ec) {
                    do_send();
                }
            });
    }

    asio::ip::udp::endpoint endpoint_;
    asio::ip::udp::socket socket_;
    asio::steady_timer timer_;
    int message_count_{0};
    std::string message_{};
};


int main(int argc, char* argv[])
{
try {
    if (argc != 2)
    {
        std::cerr << "Usage: sender <multicast_address>\n"
                  << "  For IPv4, try:\n"
                  << "    sender 239.255.0.1\n"
                  << "  For IPv6, try:\n"
                  << "    sender ff31::8000:1234\n";
        return 1;
    }

    asio::io_context context;
    Sender s{context, asio::ip::make_address(argv[1])};
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