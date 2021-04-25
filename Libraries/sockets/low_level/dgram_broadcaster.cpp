// dgram_broadcaster.cpp -- datagram sockets client that can broadcast
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
constexpr short Server_Port{4950};     // the port users will be connecting to
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
        fmt::print(stderr, "Usage: broadcaster hostname message\n");
        std::exit(1);
    }

    hostent* he = gethostbyname(argv[1]);
    if (he == nullptr)
    {
        fmt::print(stderr, "[broadcaster] gethostbyname failed {}\n", std::strerror(errno));
        std::exit(1);
    }

    const int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        fmt::print(stderr, "[broadcaster] failed to create socket {}\n", std::strerror(errno));
        std::exit(1);
    }

    // this call is what allows the broadcast packets to be sent
    int broadcast{1};
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) == -1)
    {
        fmt::print(stderr, "[broadcaster] setsockopt(SO_BROADCAST) failed {}\n", std::strerror(errno));
        std::exit(1);
    }


    sockaddr_in their_addr{};
    their_addr.sin_family = AF_INET;            // host byte order
    their_addr.sin_port = htons(Server_Port);   // short, network order
    their_addr.sin_addr = *reinterpret_cast<in_addr*>(he->h_addr);
    // std::memset(their_addr.sin_zero, '\0', sizeof(their_addr.sin_zero));

    ssize_t numbytes = sendto(sockfd, argv[2], std::strlen(argv[2]), 0,
                              reinterpret_cast<sockaddr*>(&their_addr), sizeof(their_addr));
    if (numbytes == -1)
    {
        fmt::print(stderr, "[broadcaster] sendto failed: {}\n", std::strerror(errno));
        std::exit(1);
    }

    fmt::print("[broadcaster]: sent {} bytes to {}\n", numbytes, inet_ntoa(their_addr.sin_addr));

    close(sockfd);
}
