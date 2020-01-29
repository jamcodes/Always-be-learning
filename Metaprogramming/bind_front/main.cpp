#include <iostream>

#include "bind_front.hpp"

int foo(int a, int b, int c)
{
    return a + b + c;
}

int main()
{
    auto f = bind_front(&foo, 42);
    return f(1, 2);
}
