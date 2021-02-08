// dgram_talker.cpp -- datagram sockets client
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
constexpr const char* Server_Port{"3490"};     // the port users will be connecting to
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


int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        fmt::print(stderr, "Usage: talker hostname message\n");
        std::exit(1);
    }

    addrinfo hints{};
    // std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_DGRAM;

    addrinfo* servinfo;
    if (auto const sc{getaddrinfo(argv[1], Server_Port, &hints, &servinfo)}; sc != 0)
    {
        fmt::print(stderr, "[talker] getaddrinfo: {}\n", gai_strerror(sc));
        std::exit(1);
    }

    int sockfd;
    addrinfo* p;
    for (p = servinfo; p != nullptr; p = p->ai_next)
    {
        if (sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol); sockfd == -1)
        {
            fmt::print(stderr, "[talker] socket: {}\n", std::strerror(errno));
            continue;
        }

        // could choose to connect() here - in that case the client wouldn't need to sendto()
        // but could just send() to the recieving end it connected to
    
        break;
    }

    if (p == nullptr)
    {
        fmt::print(stderr, "[talker] failed to create socket\n");
        std::exit(2);
    }

    ssize_t numbytes;
    if (numbytes = sendto(sockfd, argv[2], std::strlen(argv[2]), 0, p->ai_addr, p->ai_addrlen); numbytes == -1) 
    {
        fmt::print(stderr, "[talker] sendto: {}\n", std::strerror(errno));
        std::exit(2);
    }

    freeaddrinfo(servinfo);

    fmt::print("[talker]: sent {} bytes to {}\n", numbytes, argv[1]);
    close(sockfd);
}
