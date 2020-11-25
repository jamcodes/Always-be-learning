#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

#include <asio.hpp>


class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(asio::ip::tcp::socket socket)
        : socket_{std::move(socket)}
    {
    }

    void start()
    {
        do_read();
    }

private:
    void do_read()
    {
        socket_.async_read_some(asio::buffer(data_),
            [this, self=shared_from_this()](asio::error_code const& ec, std::size_t length)
            {
                if (!ec)
                {
                    do_write(length);
                }
                else
                {
                    std::cerr << "[Session] read error: " << ec.message() << std::endl;
                }
            }
        );
    }

    void do_write(std::size_t length)
    {
        asio::async_write(socket_, asio::buffer(data_, length),
            [this, self=shared_from_this()](asio::error_code const& ec, std::size_t /* length */)
            {
                if (!ec)
                {
                    do_read();
                }
                else
                {
                    std::cerr << "[Session] write error: " << ec.message() << std::endl;
                }
            }
        );
    }

    static constexpr std::size_t Max_Length_{1024};
    asio::ip::tcp::socket socket_;
    std::array<char, 1024> data_{};
};


class Server
{
public:
    Server(asio::io_context& context, unsigned short port)
        : acceptor_{context, asio::ip::tcp::endpoint{asio::ip::tcp::v4(), port}}
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(
            [this](std::error_code ec, asio::ip::tcp::socket socket)
            {
                if (!ec)
                {
                    std::make_shared<Session>(std::move(socket))->start();
                }
                else
                {
                    std::cerr << "[Server] accept error: " << ec.message() << std::endl;
                }
                do_accept();
            }
        );
    }

    asio::ip::tcp::acceptor acceptor_;
};


int main(int argc, char* argv[])
{
try {
    if (argc != 2)
    {
        std::cerr << "Usage: tcp_echo_server_async <port>\n";
        return 1;
    }

    asio::io_context context;
    Server server{context, static_cast<unsigned short>(std::atoi(argv[1]))};
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
