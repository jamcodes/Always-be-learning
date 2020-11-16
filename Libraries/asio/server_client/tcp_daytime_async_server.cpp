#include <ctime>
#include <chrono>
#include <iostream>
#include <string>
#include <memory>

#include <asio.hpp>


std::string make_daytime_string()
{
    std::time_t now{std::time(0)};
    return std::ctime(&now);
}


class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
    using pointer = std::shared_ptr<TcpConnection>;

    static pointer create(asio::io_context& context)
    {
        // return std::make_shared<TcpConnection>(context);
        return pointer{new TcpConnection{context}};
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

private:
    TcpConnection(asio::io_context& context)
        : socket_{context}
    {
    }

    void handle_write(asio::error_code const& ec, std::size_t /* length */)
    {
        if (ec) std::cerr << "[TcpConnection] write error: " << ec.message() << std::endl;
    }

    asio::ip::tcp::socket socket_;
    std::string message_{};
};


class TcpDaytimeServer {
public:
    TcpDaytimeServer(asio::io_context& context)
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


int main()
{
try {
    asio::io_context context;
    TcpDaytimeServer server{context};
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