#include <ctime>
#include <iostream>
#include <vector>
#include <utility>
#include <memory>

#include <asio.hpp>


// A reference-counted, non-modifiable buffer class
class SharedConstBuffer
{
    using Storage = std::vector<std::uint8_t>;
public:
    // const-buffer-sequence requirements
    using value_type = asio::const_buffer;
    using const_iterator = asio::const_buffer const*;

    SharedConstBuffer(std::string const& data)
        : data_{std::make_shared<Storage>(data.begin(), data.end())},
          buffer_{asio::buffer(*data_)}
    {
    }

    // const-buffer-sequence requirements
    const_iterator begin() const noexcept { return &buffer_; }
    const_iterator end() const noexcept { return &buffer_ + 1; }

private:
    std::shared_ptr<Storage> data_;
    asio::const_buffer buffer_;
};


class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(asio::ip::tcp::socket socket)
        : socket_{std::move(socket)}
    {
    }

    void start() { do_write(); }

private:
    void do_write()
    {
        std::time_t now{std::time(0)};
        SharedConstBuffer buffer{std::ctime(&now)};

        asio::async_write(socket_, buffer,
            [self=shared_from_this()](std::error_code ec, std::size_t /* length */)
            {
                if (ec) { std::cerr << "async_write error: " << ec.message() << std::endl; }
            }
        );
    }

    asio::ip::tcp::socket socket_;
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
                    std::cerr << "async_accept error: " << ec.message() << std::endl;
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
        std::cerr << "Usage: reference_counted <port>\n";
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
