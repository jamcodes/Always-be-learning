#pragma once

#include "net_common.h"
#include "net_message.h"
#include "net_tsqueue.h"

namespace olc
{
namespace net
{

template<typename T>
class connection : public std::enable_shared_from_this<connection<T>>
{
public:
    enum class owner
    {
        server,
        client
    };

    connection(
        owner parent,
        asio::io_context& context,
        asio::ip::tcp::socket socket,
        tsqueue<owned_message<T>>& queue_in)
        : socket_{std::move(socket)},
          context_{context},
          queue_in_{queue_in},
          owner_type_{parent}
    {
    }

    ~connection() = default;

    std::uint32_t get_id() const noexcept { return id_; }


    void connect_to_client(std::uint32_t id)
    {
        // servers connect to clients
        // contract/expectation - assert here instead?
        if (owner_type_ == owner::server)
        {
            if (socket_.is_open())
            {
                id_ = id;
                read_header();
            }
            else
            {
                std::cerr << "[SERVER]: Failed to connect to client [" << id << "]\n";
            }
        }
    }

    void connect_to_server(asio::ip::tcp::resolver::results_type const& endpoints)
    {
        // only clients can connect to servers
        // this seems like a contract that could be checked with an assert
        if (owner_type_ == owner::client)
        {
            asio::async_connect(socket_, endpoints,
                [this](asio::error_code ec, asio::ip::tcp::endpoint /* endpoint */)
                {
                    if (!ec)
                    {
                        read_header();
                    }
                    else
                    {
                        std::cerr << "Failed to connect to server: " << ec.message() << "\n";
                    }
                }
            );
        }
    }

    void disconnect()
    {
        if (is_connected())
        {
            asio::post(context_,
                [this]() { socket_.close(); }
            );
        }
    }

    bool is_connected() const noexcept
    {
        return socket_.is_open();
    }

    void send(message<T> const& msg)
    {
        // Here the asio::io_context_ is already running and waiting to read a header
        // We can post additional tasks at any time using asio::post
        asio::post(context_,
            [this, msg]()
            {
                // we need to check if the context is already busy writing messages
                // and trigger write_header only if it isn't
                const bool writing_messages{!queue_out_.empty()};
                queue_out_.push_back(msg);
                if (!writing_messages)
                {
                    write_header();
                }
            }
        );
    }

protected:
    // async - prime context ready to read a message header
    void read_header()
    {
        asio::async_read(socket_, asio::buffer(&msg_temp_in_.header, sizeof(message_header<T>)),
            [this](asio::error_code ec, std::size_t /* length */)
            {
                if (!ec)
                {
                    if (msg_temp_in_.header.size > 0)
                    {
                        msg_temp_in_.body.resize(msg_temp_in_.header.size);
                        read_body();
                    }
                    else
                    {
                        add_to_incomming_message_queue();
                    }
                }
                else
                {
                    std::cerr << "[" << id_ << "] Read Header Failed\n";
                    socket_.close();
                }
            }
        );
    }

    // async - prime context ready to read a message body
    void read_body()
    {
        asio::async_read(socket_, asio::buffer(msg_temp_in_.body.data(), msg_temp_in_.body.size()),
            [this](asio::error_code ec, std::size_t /* length */)
            {
                if (!ec)
                {
                    add_to_incomming_message_queue();
                }
                else
                {
                    std::cerr << "[" << id_ << "] Read Body Failed\n";
                    socket_.close();
                }
            }
        );
    }

    // async - prime context ready to write a message header
    void write_header()
    {
        msg_temp_out_ = queue_out_.pop_front();
        asio::async_write(socket_, asio::buffer(&msg_temp_out_.header, sizeof(msg_temp_out_.header)),
            [this](asio::error_code ec, std::size_t /* length */)
            {
                if (!ec)
                {
                    if (msg_temp_out_.body.size() != 0)
                    {
                        write_body();
                    }
                    else
                    {
                        if (!queue_out_.empty())
                        {
                            write_header();
                        }
                    }
                }
                else
                {
                    std::cerr << "[" << id_ << "] Write Header Failed\n";
                    socket_.close();
                }
            }
        );
    }

    // async - prime context ready to write a message body
    void write_body()
    {
        asio::async_write(socket_, asio::buffer(msg_temp_out_.body.data(), msg_temp_out_.body.size()),
            [this](asio::error_code ec, std::size_t /* length */)
            {
                if (!ec)
                {
                    if (!queue_out_.empty())
                    {
                        write_header();
                    }
                }
                else
                {
                    std::cerr << "[" << id_ << "] Write Body Failed\n";
                    socket_.close();
                }
            }
        );
    }

    void add_to_incomming_message_queue()
    {
        if (owner_type_ == owner::server)
        {
            queue_in_.push_back({this->shared_from_this(), msg_temp_in_});
        }
        else
        {
            queue_in_.push_back({nullptr, msg_temp_in_});
        }

        read_header();
    }

// --- member data
    // each connection has a unique socket to a remote
    asio::ip::tcp::socket socket_{};
    // the context is shared with the whole asio interface
    asio::io_context& context_;
    // queue contains all messages outgoing to the remote side of this connection
    tsqueue<message<T>> queue_out_{};
    // this queue holds all messages that have been received from the remote side
    // of this connection. Note that it's a reference - owner of this connection
    // is expected to provide a queue
    tsqueue<owned_message<T>>& queue_in_;
    // owner type decides how some of the connection behaves
    owner owner_type_{owner::server};
    std::uint32_t id_{0};

    message<T> msg_temp_in_{};
    message<T> msg_temp_out_{};
};

} // namespace net
} // namespace olc
