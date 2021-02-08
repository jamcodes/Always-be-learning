// hello_server.cpp -- a stream socket server demo
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include <fmt/core.h>


namespace
{
constexpr const char* Port{"3490"};     // the port users will be connecting to
constexpr std::uint32_t Backlog{10};    // how many pending connections can be queued
} // namespace


void sigchild_handler(int /* s */)
{
    // waitpid() might override errno; save and restore it
    const int save_errno{errno};

    while (waitpid(-1, nullptr, WNOHANG) > 0)
        ;
    
    errno = save_errno;
}

// get sockaddr, IPv4, IPv6
void* get_in_addr(sockaddr* sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(reinterpret_cast<sockaddr_in*>(sa)->sin_addr);
    }
    else
    {
        return &(reinterpret_cast<sockaddr_in6*>(sa)->sin6_addr);
    }
}


int main()
{
    addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;    // infer my IP

    addrinfo* servinfo;
    if (const int rc{getaddrinfo(nullptr, Port, &hints, &servinfo)}; rc != 0)
    {
        fmt::print(stderr, "getaddrinfo: {}\n", gai_strerror(rc));
        return 1;
    }

    int sockfd;    // listen on sock_fd, new connection on new_fd
    // loop through all the results and bind to the first we can
    addrinfo* p{servinfo};
    for (; p != nullptr; p = p->ai_next)
    {
        if (sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol); sockfd == -1)
        {
            fmt::print(stderr, "[server] socket: {}\n", std::strerror(errno));
            continue;
        }

        constexpr int yes{1};
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
        {
            fmt::print(stderr, "[server] setsockopt: {}\n", std::strerror(errno));
            std::exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            fmt::print(stderr, "[server] bind: {}\n", std::strerror(errno));
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (p == nullptr)
    {
        fmt::print(stderr, "[server]: failed to bind!\n");
        std::exit(1);
    }

    if (listen(sockfd, Backlog) == -1)
    {
        fmt::print("[server] listen: {}\n", std::strerror(errno));
        exit(1);
    }

    struct sigaction sa;
    sa.sa_handler = sigchild_handler;   // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, nullptr) == -1)
    {
        fmt::print("[server] sigaction: {}\n", std::strerror(errno));
        std::exit(1);
    }

    fmt::print("server: waiting for connections...\n");

    char addr_str[INET6_ADDRSTRLEN];
    while (1)   // main accept() loop
    {
        sockaddr_storage their_addr;
        socklen_t sin_size{sizeof(their_addr)};
        int new_fd = accept(sockfd, reinterpret_cast<sockaddr*>(&their_addr), &sin_size);
        if (new_fd == -1)
        {
            fmt::print(stderr, "[server] accept: {}\n", std::strerror);
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr(reinterpret_cast<sockaddr*>(&their_addr)),
                  addr_str, sizeof(addr_str));
        fmt::print("server: got connection from {}\n", addr_str);

        if (!fork())    // this is the child process
        {
            close(sockfd);  // child doesn't need the listener
            if (send(new_fd, "Hello, World!", 13, 0) == -1)
            {
                fmt::print(stderr, "[server] send error: {}\n", std::strerror(errno));
            }
            close(new_fd);
            std::exit(0);
        }
        else
        {
            close(new_fd);
        }
    }
}