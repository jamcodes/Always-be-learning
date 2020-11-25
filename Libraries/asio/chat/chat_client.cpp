#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>

#include <asio.hpp>

#include "chat_message.hpp"

using ChatMessageQueue = std::deque<ChatMessage>;

class ChatClient
{
public:
    ChatClient(asio::io_context& context,
        asio::ip::tcp::resolver::results_type const& endpoints)
        : context_{context},
          socket_{context}
    {
        do_connect(endpoints);
    }

    void write(ChatMessage const& msg)
    {
        asio::post(context_,
            [this, msg]()
            {
                const bool write_in_progress{!write_msgs_.empty()};
                write_msgs_.push_back(msg);
                if (!write_in_progress)
                {
                    do_write();
                }
            }
        );
    }

    void close()
    {
        asio::post(context_, [this](){ socket_.close(); });
    }

private:
    void do_connect(asio::ip::tcp::resolver::results_type const& endpoints)
    {
        asio::async_connect(socket_, endpoints,
            [this](std::error_code ec, asio::ip::tcp::endpoint)
            {
                if (!ec)
                {
                    do_read_header();
                }
                else
                {
                    std::cerr << "[ChatClient] connect error: " << ec.message() << std::endl;
                }
            }
        );
    }

    void do_read_header()
    {
        asio::async_read(socket_, asio::buffer(read_msg_.data(), ChatMessage::header_length),
            [this](std::error_code ec, std::size_t /* length */)
            {
                if (!ec)
                {
                    if (auto const decode_ec{read_msg_.decode_header()}; decode_ec == std::errc{})
                    {
                        do_read_body();
                    }
                    else
                    {
                        std::cerr << "[ChatClient] header decode error: "
                            << std::make_error_code(decode_ec).message() << std::endl;
                    }

                }
                else
                {
                    std::cerr << "[ChatClient] read header error: " << ec.message() << std::endl;
                    socket_.close();
                }
            });
    }

    void do_read_body()
    {
        asio::async_read(socket_, asio::buffer(read_msg_.body(), read_msg_.body_length()),
            [this](std::error_code ec, std::size_t /* length */)
            {
                if (!ec)
                {
                    std::cout.write(read_msg_.body(), static_cast<std::streamsize>(read_msg_.body_length()));
                    std::cout << std::endl;
                    do_read_header();
                }
                else
                {
                    std::cerr << "[ChatClient] read body error: " << ec.message() << std::endl;
                    socket_.close();
                }
            }
        );
    }

    void do_write()
    {
        asio::async_write(socket_, asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
            [this](std::error_code ec, std::size_t /* length */)
            {
                if (!ec)
                {
                    write_msgs_.pop_front();
                    if (!write_msgs_.empty())
                    {
                        do_write();
                    }
                }
                else
                {
                    std::cerr << "[ChatClient] write error: " << ec.message() << std::endl;
                    socket_.close();
                }
            }
        );
    }

    asio::io_context& context_;
    asio::ip::tcp::socket socket_;
    ChatMessage read_msg_{};
    ChatMessageQueue write_msgs_{};
};


int main(int argc, char* argv[])
{
try {
    if (argc < 2)
    {
        std::cout << "chat_client <host> <port>\n";
        return 1;
    }

    asio::io_context context;
    asio::ip::tcp::resolver resolver{context};
    auto endpoints{resolver.resolve(argv[1], argv[2])};

    ChatClient client{context, endpoints};

    std::thread client_thread{[&context](){ context.run(); }};


    for (std::array<char, ChatMessage::max_body_length + 1> line;
         std::cin.getline(line.data(), ChatMessage::max_body_length);)
    {
        ChatMessage msg;
        auto const length{std::strlen(line.data())};
        msg.body_length(length);
        std::copy(line.begin(), std::next(line.begin(), static_cast<std::ptrdiff_t>(length + 1)), msg.body());
        if (msg.encode_header() == true)
        {
            client.write(msg);
        }
        else
        {
            std::cerr << "[ChatClient]: Failed to encode header" << std::endl;
        }
    }

    client.close();
    if (client_thread.joinable()) { client_thread.join(); }
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