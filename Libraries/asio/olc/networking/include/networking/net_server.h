#pragma once

#include <limits>

#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"
#include "net_connection.h"

namespace olc
{
namespace net
{

template<typename T>
class server_interface
{
public:
    server_interface(std::uint16_t port)
        : acceptor_{context_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)}
    {

    }

    virtual ~server_interface()
    {
        stop();
    }

    bool start()
    {
        try {
            wait_for_client_connection();
            context_thread_ = std::thread{[this](){ context_.run(); }};
        }
        catch (std::exception const& e) {
            // something prohibited the server from listening
            std::cerr << "[SERVER] Exception: " << e.what() << "\n";
            return false;
        }

        std::cout << "[SERVER] Started\n";
        return true;
    }

    void stop()
    {
        context_.stop();
        if (context_thread_.joinable()) { context_thread_.join(); }
        std::cerr << "[SERVER] Stopped\n";
    }

    // async - instruct asio to wait for connection
    void wait_for_client_connection()
    {
        acceptor_.async_accept(
            [this](std::error_code ec, asio::ip::tcp::socket socket)
            {
                if (!ec) {
                    std::cerr << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";
                    auto newconn = std::make_shared<connection<T>>(
                                        connection<T>::owner::server,
                                        context_,
                                        std::move(socket),
                                        queue_in_);

                    // give the user server an opportunity to deny this connection
                    if (on_client_connect(newconn))
                    {
                        // connection allowed, so add to container of new connections
                        connections_.push_back(std::move(newconn));
                        // assign ID to the connection
                        auto&& con{connections_.back()};
                        con->connect_to_client(this, id_counter_++);
                        std::cout << "[" << con->get_id() << "] Connection Approved" << std::endl;
                    }
                    else
                    {
                        std::cout << "[------] Connection Denied\n";
                    }
                }
                else {
                    std::cerr << "[SERVER] New Connection Error: " << ec.message() << "\n";
                }

                // prime the context with more work - simply wait for another connection...
                wait_for_client_connection();
            });
    }

    // send a message to a specific client
    void message_client(std::shared_ptr<connection<T>> client, message<T> const& msg)
    {
        if (client && client->is_connected())
        {
            client->send(msg);
        }
        else
        {
            // if fail to communicate with the client assume the client has disconnected
            on_client_disconnect(client);
            client.reset();
            connections_.erase(
                std::remove(connections_.begin(), connections_.end(), client), connections_.end()
            );
        }
    }

    // send message to all clients
    void message_all_clients(message<T> const& msg, std::shared_ptr<connection<T>> ignored_client)
    {
        bool invalid_client_exists{false};
        for (auto& client : connections_)
        {
            if (client && client->is_connected())
            {
                if (client != ignored_client) {
                    client->send(msg);
                }
            }
            else
            {
                // we failed to communicate with the client, so assume it has disconnected
                on_client_disconnect(client);
                client.reset();
                invalid_client_exists = true;
            }
        }
        if (invalid_client_exists)
        {
            connections_.erase(
                std::remove(connections_.begin(), connections_.end(), nullptr), connections_.end()
            );
        }
    }

    // allow the client to decide when it's the best time to handle incomming messages
    // calling this function will process the messages
    void update(std::size_t max_messages = std::numeric_limits<std::size_t>::max(), bool wait = false)
    {
        if (wait) { queue_in_.wait_for_data(); }

        for (std::size_t msg_count{0}; msg_count != max_messages && !queue_in_.empty(); ++msg_count)
        {
            // get the front message
            auto msg{queue_in_.pop_front()};
            // pass to message handler
            on_message(msg.remote, msg.msg);
        }
    }

public:
    // called when a client is validated
    virtual void on_client_validated(std::shared_ptr<connection<T>> /* client */)
    {
    }

protected:
    // called when a client connects, the connection can be denied by returning false
    virtual bool on_client_connect(std::shared_ptr<connection<T>> const& /* client */)
    {
        return false;
    }

    // called when a client appears to have disconnected
    virtual void on_client_disconnect(std::shared_ptr<connection<T>> /* client */)
    {

    }

    // called when a message arrives
    virtual void on_message(std::shared_ptr<connection<T>> /* client */, message<T>& /* msg */)
    {

    }

// --- member data
    // threadsafe queue for incomming messages
    tsqueue<owned_message<T>> queue_in_{};

    // container of active validated objects
    std::deque<std::shared_ptr<connection<T>>> connections_{};

    asio::io_context context_{};
    std::thread context_thread_{};

    asio::ip::tcp::acceptor acceptor_{};

    // clients will be identified in the system via ID
    std::uint32_t id_counter_{10000};
};

} // namespace net

} // namespace olc
