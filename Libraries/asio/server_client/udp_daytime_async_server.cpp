#include <array>
#include <iostream>
#include <string>
#include <utility>

#include <asio.hpp>


std::string make_daytime_string()
{
    std::time_t now{std::time(0)};
    return std::ctime(&now);
}


class UdpServer
{
public:
    UdpServer(asio::io_context& context)
        : socket_{context, asio::ip::udp::endpoint{asio::ip::udp::v4(), 13}}
        {
            start_receive();
        }

private:
    void start_receive()
    {
        std::cout << "[UDP SERVER] Receiving client notifications..." << std::endl;
        socket_.async_receive_from(asio::buffer(recv_buffer_), remote_endpoint_,
            [this](asio::error_code const& ec, std::size_t length){
                handle_receive(ec, length);
            }
        );
    }

    void handle_receive(asio::error_code const& ec, std::size_t /* length */)
    {
        if (!ec)
        {
            auto message{std::make_shared<std::string>(make_daytime_string())};
            auto&& msg{*message};
            std::cout << "[UDP SERVER] Sending message to client " << remote_endpoint_.address() << std::endl;
            socket_.async_send_to(asio::buffer(msg), remote_endpoint_,
                [this, message=std::move(message)](asio::error_code const& e, std::size_t length)
                {
                    handle_send(std::move(message), e, length);
                    start_receive();
                }
            );
        }
    }

    void handle_send(std::shared_ptr<std::string> /* msg */, asio::error_code const& ec, std::size_t /* length */)
    {
        if (ec) std::cerr << "[UDP SERVER] handle-send error: " << ec.message() << std::endl;
    }

    asio::ip::udp::socket socket_;
    asio::ip::udp::endpoint remote_endpoint_{};
    std::array<char, 1> recv_buffer_{};
};


int main()
{
try {
    asio::io_context context;
    UdpServer server{context};
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