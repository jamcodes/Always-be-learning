// select_demo.cpp -- a simple demo of the select() system call
#include <cstdio>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <fmt/core.h>

namespace
{
constexpr int Stdin{0};     // File descriptor for standard input
} // namespace


int main()
{
    timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 500'000;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(Stdin, &readfds);

    // don't care about writefds and exceptfds
    select(Stdin+1, &readfds, nullptr, nullptr, &tv);

    if (FD_ISSET(Stdin, &readfds))
    {
        fmt::print("A key was pressed!\n");
    }
    else
    {
        fmt::print("Timed out.\n");
    }
}
