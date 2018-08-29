#pragma once

#if !defined(NDEBUG)
#define NDEBUG
#include <cstdio>
#define INFO(M) std::fprintf(stdout, "[DEBUG %s, %d]: " M "\n", __PRETTY_FUNCTION__, __LINE__)
#define DEBUG(M, ...) std::fprintf(stdout, "[DEBUG %s, %d]: " M "\n", \
                                   __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define INFO(M)
#define DEBUG(M,...)
#endif // NDEBUG
