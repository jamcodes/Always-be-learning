#pragma once

#include "net_common.h"
#include "net_message.h"
#include "net_tsqueue.h"
#include "net_connection.h"


namespace olc
{
namespace net
{

template<typename T>
class client_interface
{
public:
    client_interface() = default;

    ~client_interface() noexcept { disconnect(); }

    bool connect(std::string const& host, std::uint16_t port)
    {
        try {
            // resolve hostname/ip-address into tangible physical address
            asio::ip::tcp::resolver resolver{context_};
            auto endpoints = resolver.resolve(host, std::to_string(port));
            // create connection - client is the sole owner of the only one connection it makes
            connection_ = std::make_unique<connection<T>>(
                connection<T>::owner::client,
                context_,
                asio::ip::tcp::socket(context_),
                queue_in_
            );

            // tell the connection object to connect to the server
            connection_->connect_to_server(endpoints);
            // start the context thread
            context_thread_ = std::thread{[this](){ context_.run(); }};
        }
        catch (std::exception const& e) {
            std::cerr << "Client exception: " << e.what() << "\n";
            return false;
        }
        return true;
    }

    void disconnect() noexcept
    {
        if (is_connected()) {
            connection_->disconnect();
        }
        context_.stop();
        if (context_thread_.joinable()) {
            context_thread_.join();
        }
    }

    void send(message<T> const& msg)
    {
        if (is_connected())
        {
            connection_->send(msg);
        }
    }

    bool is_connected() const noexcept
    {
        if (connection_) {
            return connection_->is_connected();
        }
        else {
            return false;
        }
    }

    tsqueue<owned_message<T>>& incomming() noexcept
    {
        return queue_in_;
    }

private:
    // asio context and the dedicated thread
    asio::io_context context_{};
    std::thread context_thread_{};
    // hardware socket connected to the server
    asio::ip::tcp::socket socket_{context_};
    // the client has a single connection object, which handles data transfer
    std::unique_ptr<connection<T>> connection_{};
    // queue of messages incomming from server
    tsqueue<owned_message<T>> queue_in_{};
};

} // namespace net
} // namespace olc
