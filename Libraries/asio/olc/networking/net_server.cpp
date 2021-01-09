#include <iostream>

#include <olc_net.h>


enum class CustomMsgTypes : std::uint32_t
{
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage
};

class CustomServer : public olc::net::server_interface<CustomMsgTypes>
{
public:
    CustomServer(std::uint16_t port) : olc::net::server_interface<CustomMsgTypes>{port}
    {

    }

protected:
    virtual bool on_client_connect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> const& client) override
    {
        std::cerr << "on_client_connect: [" << client->get_id() << "]\n";
        return true;
    }

    // called when a client appears to have disconnected
    virtual void on_client_disconnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client) override
    {
        std::cerr << "on_client_disconnect: [" << client->get_id() << "]\n";
    }

    // called when a message arrives
    virtual void on_message(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client,
                            olc::net::message<CustomMsgTypes>& msg) override
    {
        switch (msg.header.id)
        {
        case CustomMsgTypes::ServerPing:
        {
            std::cerr << "[" << client->get_id() << "]: Server Ping\n";
            // simply bounce message back to client
            client->send(msg);
            break;
        }
        case CustomMsgTypes::MessageAll:
        {
            std::cerr << "[" << client->get_id() << "]: Message All\n";
            olc::net::message<CustomMsgTypes> msg_all;
            msg_all.header.id = CustomMsgTypes::ServerMessage;
            msg_all << client->get_id();
            message_all_clients(msg_all, client);
            break;
        }
        default:
        {
            std::cerr << "[" << client->get_id() << "]: Unknown Message Type\n";
        }
        }
    }
};


int main()
{
    CustomServer server{60000};
    server.start();

    while (true)
    {
        server.update(std::numeric_limits<std::size_t>::max(), true);
    }
}