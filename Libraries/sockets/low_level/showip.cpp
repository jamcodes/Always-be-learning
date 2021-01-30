// showip.cpp - show IP addresses for a host given on the command line
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <fmt/core.h>


int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fmt::print(stderr, "usage: showip hostname\n");
        return 1;
    }

    addrinfo hints{};
    addrinfo* res{nullptr};
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;        // IP-version-agnostic, AF_INET, or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;    // 

    int status = getaddrinfo(argv[1], nullptr, &hints, &res);
    if (status != 0)
    {
        fmt::print(stderr, "getaddrinfo: {}\n", gai_strerror(status));
        return 2;
    }

    fmt::print("IP addresses for {}:\n\n", argv[1]);

    for (auto p{res}; p != nullptr; p = p->ai_next)
    {
        void* addr;
        const char* ipver;

        // get the pointer to the address itself
        // different fields in IPv4 and IPv6
        if (p->ai_family == AF_INET) {  // IPv4
            sockaddr_in* ipv4 = reinterpret_cast<sockaddr_in*>(p->ai_addr);
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        }
        else    // IPv6
        {
            sockaddr_in6* ipv6 = reinterpret_cast<sockaddr_in6*>(p->ai_addr);
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        fmt::print("  {}: {}\n", ipver, ipstr);
    }

    freeaddrinfo(res); 
}
