// selectserver.cpp -- a multiclient chat based on select()
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


namespace
{
constexpr const char* Port{"9034"};   // port the server is listening on
} // namespace


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


int main()
{
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    addrinfo* ai;
    if (auto const sc{getaddrinfo(nullptr, Port, &hints, &ai)}; sc != 0)
    {
        fmt::print(stderr, "selectserver: {}\n", gai_strerror(sc));
        std::exit(1);
    }

    addrinfo* p;
    int listener;       // listening socket descriptor
    for (p = ai; p != nullptr; p = p->ai_next)
    {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) { continue; }

        int yes{1};
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
        {
            close(listener);
            continue;
        }

        break;
    }

    if (p == nullptr)
    {
        fmt::print(stderr, "selectserver: failed to bind {}\n", std::strerror(errno));
        std::exit(2);
    }

    freeaddrinfo(ai);

    // listen
    if (listen(listener, 10) == -1)
    {
        fmt::print(stderr, "selectserver: listen failure {}\n", std::strerror(errno));
        std::exit(3);
    }

    fd_set master;       // master file descriptor list
    FD_ZERO(&master);    // clear the master and temp sets
    // add the listener to the master set
    FD_SET(listener, &master);
    // keep track of the biggest fd
    int maxfd;           // maximum file descriptor number
    maxfd = listener;

    fmt::print("selectserver: listening for connections...\n");
    char remoteIP[INET6_ADDRSTRLEN];
    // main loop
    for (;;)
    {
        fd_set read_fds;     // temp file descriptor list for select()
        FD_ZERO(&read_fds);
        read_fds = master;
        if (select(maxfd + 1, &read_fds, nullptr, nullptr, nullptr) == -1)
        {
            fmt::print(stderr, "selectserver: select failed {}\n", std::strerror(errno));
            std::exit(4);
        }

        // run through the existing connections looking for data
        fmt::print("selectserver: scanning connections for data...\n");
        for (int i{0}; i <= maxfd; ++i)
        {
            if (FD_ISSET(i, &read_fds))
            {
                if (i == listener)
                {
                    // handle new connection
                    sockaddr_storage remoteaddr;    // client address
                    socklen_t addrlen{sizeof(remoteaddr)};

                    const int newfd = accept(listener, reinterpret_cast<sockaddr*>(&remoteaddr), &addrlen);

                    if (newfd == -1)
                    {
                        fmt::print(stderr, "selectserver: accept failed {}\n", std::strerror(errno));
                    }
                    else
                    {
                        FD_SET(newfd, &master);     // add to master
                        if (newfd > maxfd)
                        {
                            maxfd = newfd;
                        }
                        fmt::print("selectserver: new connection from {} on socket {}\n",
                            inet_ntop(remoteaddr.ss_family, get_in_addr(reinterpret_cast<sockaddr*>(&remoteaddr)),
                                      remoteIP, INET6_ADDRSTRLEN),
                            newfd);
                    }
                }
                else
                {
                    // handle data from client
                    char buf[256];
                    ssize_t nbytes = recv(i, buf, sizeof(buf), 0);
                    if (nbytes <= 0)
                    {
                        // error or connection closed
                        if (nbytes == 0)
                        {
                            // connection closed
                            fmt::print("selectserver: socket {} hung up\n", i);
                        }
                        else
                        {
                            fmt::print(stderr, "selectserver: recv failure {}\n", std::strerror(errno));
                        }
                        close(i);
                        FD_CLR(i, &master);
                    }
                    else
                    {
                        // got some good data from client
                        for (int j{0}; j != maxfd; ++j)
                        {
                            // send to everyone
                            if (FD_ISSET(j, &master))
                            {
                                // except listener and ourselves
                                if (j != listener && j != i)
                                {
                                    if (send(j, buf, static_cast<size_t>(nbytes), 0) == -1)
                                    {
                                        fmt::print(stderr, "selectserver: send failure {}\n", std::strerror(errno));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
