#include <array>
#include <chrono>
#include <iostream>
#include <atomic>
#include <thread>
#include <future>

#include <olc_net.h>


enum class CustomMsgTypes : std::uint32_t
{
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
};

struct Foo {
        float x;
        float y;
};

class CustomClient : public olc::net::client_interface<CustomMsgTypes>
{
    using Message = olc::net::message<CustomMsgTypes>;
public:
    void ping_server()
    {
        olc::net::message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::ServerPing;

        auto time_now{std::chrono::steady_clock::now()};
        msg << time_now;
        send(msg);
    }

    void message_all()
    {
        // send a message to all other clients
        olc::net::message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::MessageAll;
        send(msg);
    }
};

int main()
{
    // olc::net::message<CustomMsgTypes> msg;
    // msg.header.id = CustomMsgTypes::FireBullet;

    // int a{1};
    // bool b{true};
    // float c{3.14159f};
    // std::array<Foo, 5> d{{{1.1, 1.2}, {2.1, 2.2}, {3.1, 3.2}, {4.1, 4.2}, {5.1, 5.2}}};

    // msg << a << b << c << d;

    // a = 99;
    // b = false;
    // c = 99.0f;
    // d.fill({0.0, 0.0});

    // msg >> d >> c >> b >> a;

    enum class Ops : std::uint32_t
    {
        Idle,
        Ping,
        All,
        Quit
    };

    std::atomic_uint32_t op{static_cast<std::uint32_t>(Ops::Idle)};

    std::thread client_thread{
        [&op]() {
            CustomClient client;
            auto connected = client.connect("127.0.0.1", 60000);
            bool quit{false};
            if (!connected)
            {
                std::cerr << "Failed to connect to the server\n";
                quit = true;
            }
            while (!quit)
            {
                Ops const command{op.exchange(static_cast<std::uint32_t>(Ops::Idle))};
                if (command == Ops::Ping) { client.ping_server(); }
                if (command == Ops::All) { client.message_all(); }
                if (command == Ops::Quit) { quit = true; break; }
                if (client.is_connected())
                {
                    if (!client.incomming().empty())
                    {
                        auto msg = client.incomming().pop_front().msg;
                        switch (msg.header.id)
                        {
                            case CustomMsgTypes::ServerAccept:
                            {
                                std::cerr << "Server Accepted Connection\n";
                                break;
                            }
                            case CustomMsgTypes::ServerPing:
                            {
                                auto const time_now{std::chrono::steady_clock::now()};
                                std::chrono::steady_clock::time_point time_then;
                                msg >> time_then;
                                std::cerr << "Ping: " << std::chrono::duration<double>(time_now - time_then).count() << "\n";
                                break;
                            }
                            case CustomMsgTypes::ServerMessage:
                            {
                                // id of a client sent a broadcast to all other clients
                                std::uint32_t client_id;
                                msg >> client_id;
                                std::cerr << "Hello from [" << client_id << "]\n";
                                break;
                            }
                            default:
                            {
                                std::cerr << "Unknown message type\n";
                                // quit = true;
                            }
                        }
                    }
                }
                else
                {
                    std::cerr << "Server Down\n";
                    quit = true;
                }
            }
        }
    };

    for (std::string line{}; std::cout << ">> ", getline(std::cin, line); line != "quit")
    {
        if (line == "ping") {
            op.store(static_cast<std::uint32_t>(Ops::Ping));
        }
        if (line == "all") {
            op.store(static_cast<std::uint32_t>(Ops::All));
        }
        if (line == "quit") {
            op.store(static_cast<std::uint32_t>(Ops::Quit));
            std::cerr << "exiting..." << std::endl;
            break;
        }
    }

    client_thread.join();
}
