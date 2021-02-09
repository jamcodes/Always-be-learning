#include <cstdio>
#include <poll.h>

#include <fmt/core.h>


int main()
{
    pollfd pfds[1];     // as many in the array as you need to monitor

    pfds[0].fd = 0;             // standard input
    pfds[0].events = POLLIN;    // Tell me when ready to read

    // if you need to monitor other things as well
    // pfds[1].fd = some_sockfd;    // monitor some socket 
    // pfds[1].events = POLLIN;     // Notify when ready to read

    fmt::print("Hit return to wait 2.5 seconds for timeout\n");

    const int num_events = poll(pfds, 1, 2500);    // 2.5 seconds timeout
    if (num_events == 0)
    {
        fmt::print("Poll timed out!\n");
    }
    else
    {
        if (auto const pollin_happened = pfds[0].revents & POLLIN; pollin_happened)
        {
            fmt::print("File descriptor {} is ready to read\n", pfds[0].fd);
        }
        else
        {
            fmt::print("Unexpected event occured: {}\n", pfds[0].revents);
        }
    }
}
