#include <iostream>
#include <array>
#include <chrono>

#include <asio.hpp>


std::string make_daytime_string()
{
    std::time_t now{std::time(0)};
    return std::ctime(&now);
}


class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
    struct private_tag { };
public:
    using pointer = std::shared_ptr<TcpConnection>;

    static pointer create(asio::io_context& context)
    {
        return std::make_shared<TcpConnection>(private_tag{}, context);
        // return pointer{new TcpConnection{context}};
    }

    asio::ip::tcp::socket& socket() noexcept
    {
        return socket_;
    }

    void start()
    {
        message_ = make_daytime_string();
        asio::async_write(socket_, asio::buffer(message_),
            [p=shared_from_this()](asio::error_code const& ec, std::size_t length){
                p->handle_write(ec, length);
            }
        );
    }

    TcpConnection(private_tag, asio::io_context& context)
        : socket_{context}
    {
    }
private:

    void handle_write(asio::error_code const& ec, std::size_t /* length */)
    {
        if (ec) std::cerr << "[TcpConnection] write error: " << ec.message() << std::endl;
    }

    asio::ip::tcp::socket socket_;
    std::string message_{};
};


class TcpServer {
public:
    TcpServer(asio::io_context& context)
        : context_{context},
          acceptor_{context, asio::ip::tcp::endpoint{asio::ip::tcp::v4(), 13}}
        {
            start_accept();
        }
private:
    void start_accept()
    {
        std::cout << "[SERVER] Accepting new connections..." << std::endl;
        TcpConnection::pointer new_connection{TcpConnection::create(context_)};
        auto&& socket{new_connection->socket()};
        acceptor_.async_accept(socket, [this, conn=std::move(new_connection)](asio::error_code const& ec){
            handle_accept(std::move(conn), ec);
        });
    }

    void handle_accept(TcpConnection::pointer conn, asio::error_code const& ec)
    {
        if (!ec) {
            std::cout << "[SERVER] Got New connection" << std::endl;
            conn->start();
        }
        else {
            std::cerr << "[SERVER] error: " << ec.message() << std::endl;
        }

        start_accept();
    }

    asio::io_context& context_;
    asio::ip::tcp::acceptor acceptor_;
};

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
    TcpServer tcp_server{context};
    UdpServer udp_server{context};
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