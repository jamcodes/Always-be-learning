#pragma once

#include "net_common.h"
#include "net_message.h"
#include "net_tsqueue.h"

namespace olc
{
namespace net
{

// forward declaration
template<typename T> class server_interface;

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
        if (owner_type_ == owner::server)
        {
            // connection is server -> client, construct random data for the client
            // to transform and send back for validation
            handshake_out_ = static_cast<std::uint64_t>(std::chrono::system_clock::now().time_since_epoch().count());
            // pre-calculated result for checking when the client responds
            handshake_check_ = scramble(handshake_out_);
        }
        else
        {
            // connection is client -> server, there's nothing to define - wait for the server to initiate validation
            handshake_in_ = 0;
            handshake_out_ = 0;
        }
    }

    ~connection() = default;

    std::uint32_t get_id() const noexcept { return id_; }


    void connect_to_client(olc::net::server_interface<T>* server, std::uint32_t id)
    {
        // servers connect to clients
        // contract/expectation - assert here instead?
        if (owner_type_ == owner::server)
        {
            if (socket_.is_open())
            {
                id_ = id;
                // read_header();

                // a client has attempted to connect to the server - we want to validate it
                // send out the handshare data first
                write_validation();

                // wait for the validation data to be sent back from the client
                read_validation(server);
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
                        // read_header();

                        // the server validates the client
                        // read the validation data
                        read_validation();
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

    // ASYNC - used by both client and server to write validation packet
    void write_validation()
    {
        asio::async_write(socket_, asio::buffer(&handshake_out_, sizeof(handshake_out_)),
            [this](std::error_code ec, std::size_t /* length */)
            {
                if (!ec)
                {
                    // validation data sent - clients start attempting to read data now
                    if (owner_type_ == owner::client)
                    {
                        read_header();
                    }
                }
                else
                {
                    std::cerr << "[Connection] write validation failed: " << ec.message() << std::endl;
                    socket_.close();
                }
            }
        );
    }

    void read_validation(olc::net::server_interface<T>* server = nullptr)
    {
        asio::async_read(socket_, asio::buffer(&handshake_in_, sizeof(handshake_in_)),
            [this, server](std::error_code ec, std::size_t /* length */)
            {
                if (!ec)
                {
                    if (owner_type_ == owner::server)
                    {
                        // expect the response from a client
                        // needs to match the precalculated result
                        if (handshake_in_ == handshake_check_)
                        {
                            // client responded with a valid solution
                            std::cout << "[Connection] Client validated" << std::endl;
                            server->on_client_validated(this->shared_from_this());

                            // move on to reading the data
                            read_header();
                        }
                        else
                        {
                            // client responded with incorrect data - refuse connection
                            std::cout << "[Connection] Client validation failed, connection refused" << std::endl;
                            socket_.close();
                            // we could track missbehaving clients, maybe blacklist them
                        }
                    }
                    else
                    {
                        // expect random data from the server
                        // encrypt it
                        handshake_out_ = scramble(handshake_in_);
                        // and send it back to the server
                        write_validation();
                    }
                }
                else
                {
                    std::cerr << "[Connection] read validation - read failure: " << ec.message() << std::endl;
                    socket_.close();
                }
            }
        );
    }

    // ad-hoc "encryption" of data
    static constexpr std::uint64_t scramble(std::uint64_t input) noexcept
    {
        std::uint64_t out{input ^ 0xDEADBEEFC0DECAFE};
        out = (out & 0xF0F0F0F0F0F0F0) >> 4 | (out & 0x0F0F0F0F0F0F0F) << 4;
        return out ^ 0xC0DEFACE12345678;
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

    std::uint64_t handshake_out_{0};
    std::uint64_t handshake_in_{0};
    std::uint64_t handshake_check_{0};
};

} // namespace net
} // namespace olc
