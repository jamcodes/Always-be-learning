// hello_client.cpp -- a stream socket client
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
constexpr const char* Port{"3490"};     // the port users will be connecting to
constexpr std::uint32_t Max_Data_Size{128};    // Max number of bytes we accept at once
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
    if (argc != 2)
    {
        fmt::print(stderr, "usage: client hostname\n");
        std::exit(1);
    }

    addrinfo hints{};
    // std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    addrinfo* servinfo;
    if (auto const sc{getaddrinfo(argv[1], Port, &hints, &servinfo)}; sc != 0)
    {
        fmt::print(stderr, "[client] getaddrinfo: {}\n", gai_strerror(sc));
        std::exit(1);
    }

    // loop through all of the results and connect to the first one possible
    addrinfo* p;
    int sockfd;
    for (p = servinfo; p != nullptr; p = p->ai_next)
    {
        if (sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol); sockfd == -1)
        {
            fmt::print(stderr, "[client] socket: {}\n", std::strerror(errno));
            continue;
        }
        
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            fmt::print(stderr, "[client] connect: {}\n", std::strerror(errno));
            continue;
        }

        break;
    }

    if (p == nullptr)
    {
        fmt::print(stderr, "[client] failed to connect\n");
        std::exit(2);
    }

    char addr_str[INET6_ADDRSTRLEN];
    inet_ntop(p->ai_family, get_in_addr(reinterpret_cast<sockaddr*>(p->ai_addr)), addr_str, sizeof(addr_str));

    fmt::print("[client] connecting to {}\n", addr_str);
    freeaddrinfo(servinfo);

    ssize_t numbytes;
    char buf[Max_Data_Size];
    if (numbytes = recv(sockfd, buf, Max_Data_Size-1, 0); numbytes == -1)
    {
        fmt::print(stderr, "[client] recv error {}\n", std::strerror(errno));
        std::exit(1);
    }

    buf[numbytes] = '\0';
    fmt::print("[client] received {}\n", buf);
    
    close(sockfd);
}
