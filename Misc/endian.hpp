#pragma once

// Endian detection utility, relying on the compiler as implemented by
// Howard Hinnant https://howardhinnant.github.io/endian.html
#include <cstdint>

enum class Endian
{
#ifdef _WIN32
    little = 0,
    big    = 1,
    native = little
#else
    little = __ORDER_LITTLE_ENDIAN__,
    big    = __ORDER_BIG_ENDIAN__,
    native = __BYTE_ORDER__
#endif
};
