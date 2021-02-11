// float_serialization.cpp -- a quick and dirty implementation of float serialization
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <fmt/core.h>

uint32_t htonf(float f)
{
    uint32_t sign;

    if (f < 0)
    {
        sign = 1;
        f = -f;
    }
    else
    {
        sign = 0;
    }

    // uint32_t p;
    // std::memcpy(&p, &f, sizeof(f));
    uint32_t p = ((uint32_t(f) & 0x7fff) << 16) | (sign << 31); // whole part and sign
    p |= (uint32_t((f - int(f)) * 65536.0f)) & 0xffff;          // fraction

    return p;
}

float ntohf(uint32_t p)
{
    float f = (p >> 16) & 0x7fff;   // whole part
    f += (p&0xffff) / 65536.0f;     // fraction

    if (((p >> 31) & 0x1) == 0x1)   // if sign bit set
    {
        f = -f;
    }

    return f;
}


int main()
{
    float f1{3.1415926};
    
    const uint32_t netf = htonf(f1);  // serialize to "network" format
    const float f2 = ntohf(netf);     // convert back

    fmt::print("Original: {}\n", f1);       // 3.141593
    fmt::print(" Network: 0x{0:x}", netf);  // 0x0003243F
    fmt::print("Unpacked: {0}\n", f2);      // 3.141586

    return 0;
}
