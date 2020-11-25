#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>

#include <asio.hpp>

#include "chat_message.hpp"


using ChatMessageQueue = std::deque<ChatMessage>;


class ChatParticipant
{
public:
    using Pointer = std::shared_ptr<ChatParticipant>;
    ChatParticipant() noexcept = default;
    ChatParticipant(ChatParticipant const&) noexcept = default;
    ChatParticipant(ChatParticipant&&) noexcept = default;
    ChatParticipant& operator=(ChatParticipant const&) noexcept = default;
    ChatParticipant& operator=(ChatParticipant&&) noexcept = default;
    virtual ~ChatParticipant() = default;

    virtual void deliver(ChatMessage const&) = 0;

private:
};


class ChatRoom
{
public:
    void join(ChatParticipant::Pointer participant)
    {
        auto&& p{*participant};
        participants_.insert(std::move(participant));
        for (auto&& msg : recent_msgs_)
        {
            p.deliver(msg);
        }
    }

    void leave(ChatParticipant::Pointer participant)
    {
        participants_.erase(participant);
    }

    void deliver(ChatMessage const& msg)
    {
        recent_msgs_.push_back(msg);
        while (recent_msgs_.size() > Max_Recent_Msgs)
        {
            recent_msgs_.pop_front();
        }

        for (auto&& p : participants_)
        {
            p->deliver(msg);
        }
    }

private:
    static constexpr std::size_t Max_Recent_Msgs{100};
    std::set<ChatParticipant::Pointer> participants_{};
    ChatMessageQueue recent_msgs_{};
};


class ChatSession : public ChatParticipant,
                    public std::enable_shared_from_this<ChatSession>
{
public:
    ChatSession(asio::ip::tcp::socket socket, ChatRoom& room)
        : socket_{std::move(socket)}, room_{room}
    {
    }

    virtual ~ChatSession() = default;

    void start()
    {
        room_.join(shared_from_this());
        do_read_header();
    }

    virtual void deliver(ChatMessage const& msg) override
    {
        const bool write_in_progress{!write_msgs_.empty()};
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
            do_write();
        }
    }

protected:
    ChatSession(ChatSession const&) = default;
    ChatSession(ChatSession&&) noexcept = default;
    ChatSession& operator=(ChatSession const&) = delete;
    ChatSession& operator=(ChatSession&&) = delete;

private:
    void do_read_header()
    {
        asio::async_read(socket_,
            asio::buffer(read_msg_.data(), ChatMessage::header_length),
            [this, self=shared_from_this()](std::error_code ec, std::size_t /* length */)
            {
                if (!ec)
                {
                    if (auto decode_ec{read_msg_.decode_header()}; decode_ec == std::errc{}){
                        do_read_body();
                    }
                    else
                    {
                        std::cerr << "[ChatServer] Session - failed to decode header: "
                            << std::make_error_code(decode_ec).message() << std::endl;
                    }
                }
                else
                {
                    std::cerr << "[ChatServer] Session - failed to read header: " << ec.message() << std::endl;
                    room_.leave(shared_from_this());
                    // room_.leave(self);
                }
            }
        );
    }

    void do_read_body()
    {
        asio::async_read(socket_,
            asio::buffer(read_msg_.body(), read_msg_.body_length()),
            [this, self=shared_from_this()](std::error_code ec, std::size_t /* length */)
            {
                if (!ec)
                {
                    room_.deliver(read_msg_);
                    do_read_header();
                }
                else
                {
                    std::cerr << "[ChatServer] Session - failed to read header: " << ec.message() << std::endl;
                    room_.leave(shared_from_this());
                    // room_.leave(self);
                }
            }
        );
    }

    void do_write()
    {
        asio::async_write(socket_,
            asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
            [this, self=shared_from_this()](std::error_code ec, std::size_t /* length */)
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
                    std::cerr << "[ChatServer] Session - failure to write message: " << ec.message() << std::endl;
                    room_.leave(shared_from_this());
                    // room_.leave(self);
                }
            }
        );
    }

    asio::ip::tcp::socket socket_;
    ChatRoom& room_;
    ChatMessage read_msg_{};
    ChatMessageQueue write_msgs_{};
};


class ChatServer
{
public:
    ChatServer(asio::io_context& context,
               asio::ip::tcp::endpoint const& endpoint)
        : acceptor_{context, endpoint}
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
                    std::make_shared<ChatSession>(std::move(socket), room_)->start();
                }
                else
                {
                    std::cerr << "[ChatServer] accept error: " << ec.message() << std::endl;
                }

                do_accept();
            }
        );
    }

    asio::ip::tcp::acceptor acceptor_;
    ChatRoom room_{};
};


int main(int argc, char* argv[])
{
try {
    if (argc < 2) {
        std::cerr << "usage: chat_server <port> [<port>...]\n";
        return 1;
    }

    asio::io_context context;

    std::list<ChatServer> servers;
    for (int i{1}; i < argc; ++i)
    {
        asio::ip::tcp::endpoint endpoint{asio::ip::tcp::v4(), static_cast<unsigned short>(std::atoi(argv[i]))};
        servers.emplace_back(context, endpoint);
    }

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
