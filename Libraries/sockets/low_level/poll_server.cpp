// poll_server.cpp -- a telnet chat server based on poll()
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <vector>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <fmt/core.h>

namespace {
constexpr const char* Port{"9034"};  // Port we're listening on
}  // namespace

// get sockaddr, IPv4, IPv6
void* get_in_addr(sockaddr* sa)
{
    if (sa->sa_family == AF_INET) {
        return &(reinterpret_cast<sockaddr_in*>(sa)->sin_addr);
    }
    else {
        return &(reinterpret_cast<sockaddr_in6*>(sa)->sin6_addr);
    }
}

// get a listening socket
int get_listener_socket() noexcept
{
    int yes{1};  // For setsockopt SO_REUSEADDR

    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    addrinfo* ai;
    if (auto const sc{getaddrinfo(nullptr, Port, &hints, &ai)}; sc != 0)
    {
        fmt::print(stderr, "[poll server] getaddrinfo: {}\n", gai_strerror(sc));
        std::exit(1);
    }

    int listener;
    addrinfo* p;
    for (p = ai; p != nullptr; p = p->ai_next)
    {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) { continue; }

        // reuse sockets
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
        
        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
        {
            close(listener);
            continue;
        }

        break;
    }

    freeaddrinfo(ai);

    if (p == nullptr)
    {
        fmt::print("[poll server] failed to bind address\n");
        return -1;
    }

    if (listen(listener, 10) == -1)
    {
        return -1;
    }

    return listener;
}

void del_from_pfds(std::vector<pollfd>& pfds, std::size_t i)
{
    pfds[i] = pfds.back();
    pfds.pop_back();
}

constexpr pollfd make_pollfd(int fd, short events = POLLIN) noexcept
{
    pollfd pfd{};
    pfd.fd = fd;
    pfd.events = events;
    return pfd;
}

int main()
{
    char buf[256];
    char remote_ip[INET6_ADDRSTRLEN];

    std::vector<pollfd> pfds{};
    pfds.reserve(8);
    const int listener = get_listener_socket();
    if (listener == -1)
    {
        fmt::print(stderr, "[poll server] error getting listening socket\n");
        std::exit(1);
    }

    pfds.push_back(make_pollfd(listener));
    for (;;)
    {
        int poll_count = poll(pfds.data(), pfds.size(), -1);
        if (poll_count == -1)
        {
            fmt::print(stderr, "[poll server] poll error {}\n", std::strerror(errno));
            std::exit(1);
        }

        // Run through the existing collections looking for data
        for (std::size_t i{0}; i != pfds.size(); ++i)
        {
            // check if ready to read
            if (pfds[i].revents & POLLIN)
            {
                if (pfds[i].fd == listener)
                {
                    // If listener is ready to read, handle new
                    struct sockaddr_storage remoteaddr; // Client address
                    socklen_t addrlen{sizeof(remoteaddr)};
                    const int newfd = accept(listener, reinterpret_cast<sockaddr*>(&remoteaddr), &addrlen);
                    if (newfd == -1)
                    {
                        fmt::print(stderr, "[poll server] accept error: {}\n", std::strerror(errno));
                    } 
                    else
                    {
                        pfds.push_back(make_pollfd(newfd));
                        fmt::print("[poll server]: new connection from {} on socket {}\n",
                            inet_ntop(remoteaddr.ss_family,
                                get_in_addr(reinterpret_cast<sockaddr*>(&remoteaddr)), remote_ip, INET6_ADDRSTRLEN),
                            newfd);
                    }
                }
                else
                {
                    // if not the listener, we're just a regular client
                    ssize_t nbytes = recv(pfds[i].fd, buf, sizeof(buf), 0);
                    const int sender_fd{pfds[i].fd};

                    if (nbytes <= 0)    // got error or connection closed by client
                    {
                        if (nbytes == 0)
                        {
                            // connection closed
                            fmt::print("[poll server] socket {} hung up\n", sender_fd);
                        }
                        else
                        {
                            fmt::print(stderr, "[poll server] recv error {}\n", std::strerror(errno));
                        }

                        close(pfds[i].fd);  // bye!
                        del_from_pfds(pfds, i);
                    }
                    else
                    {
                        // we got some good data from the client
                        for (std::size_t j{0}; j != pfds.size(); ++j)
                        {
                            // send to everyone
                            const int dest_fd{pfds[j].fd};
                            // except the listener and ourselves
                            if (dest_fd != listener && dest_fd != sender_fd)
                            {
                                if (send(dest_fd, buf, static_cast<std::size_t>(nbytes), 0) == -1)
                                {
                                    fmt::print(stderr, "[poll server] send error {}\n", std::strerror(errno));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
