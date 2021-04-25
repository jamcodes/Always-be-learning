// dgram_listener.cpp -- datagram sockets server
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

#include <fmt/core.h>

namespace
{
constexpr const char* Port{"4950"};     // the port users will be connecting to
constexpr std::uint32_t Max_Buf_Len{128};    // how many pending connections can be queued
} // namespace

// get sockaddr, IPv4, IPv6
void* get_in_addr(sockaddr* sa) noexcept
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
    addrinfo hints{};
    // std::memset(&hints; 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    addrinfo* servinfo;
    if (auto const sc{getaddrinfo(nullptr, Port, &hints, &servinfo)}; sc != 0)
    {
        fmt::print(stderr, "[server] getaddrinfo: {}\n", gai_strerror(sc));
        std::exit(1);
    }

    int sockfd;
    addrinfo* p;
    for (p = servinfo; p != nullptr; p = p->ai_next)
    {
        if (sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol); sockfd == -1)
        {
            fmt::print(stderr, "[server] socket: {}\n", std::strerror(errno));
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            fmt::print(stderr, "[server] bind: {}\n", std::strerror(errno));
            continue;
        }

        break;
    }

    if (p == nullptr)
    {
        fmt::print(stderr, "[socket] failed to bind to socket\n");
        std::exit(2);
    }

    freeaddrinfo(servinfo);

    fmt::print("[server]: waiting to recvfrom...\n");

    ssize_t numbytes;
    sockaddr_storage their_addr;
    char buf[Max_Buf_Len];
    char addr_str[INET6_ADDRSTRLEN];

    socklen_t addr_len{sizeof(their_addr)};
    if (numbytes = recvfrom(sockfd, buf, Max_Buf_Len-1, 0, reinterpret_cast<sockaddr*>(&their_addr), &addr_len);
        numbytes == -1)
    {
        fmt::print(stderr, "[server] recvfrom {}\n", std::strerror(errno));
        std::exit(1);
    }

    fmt::print("[server]: got packet from {}\n",
        inet_ntop(their_addr.ss_family, get_in_addr(reinterpret_cast<sockaddr*>(&their_addr)), addr_str, sizeof(addr_str)));
    fmt::print("[server] packet is {} bytes long\n", numbytes);
    buf[numbytes] = '\0';
    fmt::print("listener: packet contains \"{}\"\n", buf);

    close(sockfd);
}
