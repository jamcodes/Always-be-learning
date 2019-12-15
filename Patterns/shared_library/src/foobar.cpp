#include "foobar/foobar.h"

#include <cstdio>

void Foo::do_bar() noexcept
{
    std::puts("Hello shared libraries");
}
