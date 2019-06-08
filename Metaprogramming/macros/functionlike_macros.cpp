/**************************************************************************************************/

#include <iostream>

/***************************************************************************************************

# How to use this document

The code can be compiled simply:

```shell
clang++ -DBMBF_PRODUCT_CONFIG=DESKTOP better_macros_better_flags.cpp && ./a.out
```

It should print out a series of notes about compilation state.

`BMBF_PRODUCT_CONFIG` can be any one of `DESKTOP`, `MOBILE`, or `EMBEDDED`, and will produce
slightly different output.

***************************************************************************************************/

/*
    PRODUCT macros denote what product (target) is being built. There is exactly one product defined
    per translation unit. The product macro is defined at the project level, and must precede any
    preprocessing.
*/

#define BMBF_PRODUCT_PRIVATE_DEFINITION_DESKTOP() 0
#define BMBF_PRODUCT_PRIVATE_DEFINITION_MOBILE() 1
#define BMBF_PRODUCT_PRIVATE_DEFINITION_EMBEDDED() 2
#define BMBF_PRODUCT_XSMASH(X, Y) X##Y()
#define BMBF_PRODUCT_SMASH(X, Y) BMBF_PRODUCT_XSMASH(X, Y)
#define BMBF_PRODUCT(X) \
    (BMBF_PRODUCT_PRIVATE_DEFINITION_##X() == \
        BMBF_PRODUCT_SMASH(BMBF_PRODUCT_PRIVATE_DEFINITION_, BMBF_PRODUCT_CONFIG))

/**************************************************************************************************/

/*
    PLATFORM macros denote operating-system- or machine-level features. They are automatically
    derived based on built-in preprocessor definitions defined at compile-time. It is common to have
    more than one platform defined per translation unit.
*/

#define BMBF_PLATFORM_PRIVATE_DEFINITION_APPLE() 0
#define BMBF_PLATFORM_PRIVATE_DEFINITION_IOS() 0
#define BMBF_PLATFORM_PRIVATE_DEFINITION_IOS_SIMULATOR() 0
#define BMBF_PLATFORM_PRIVATE_DEFINITION_LINUX() 0
#define BMBF_PLATFORM_PRIVATE_DEFINITION_MACOS() 0
#define BMBF_PLATFORM_PRIVATE_DEFINITION_MICROSOFT() 0
#define BMBF_PLATFORM_PRIVATE_DEFINITION_POSIX() 0
#define BMBF_PLATFORM_PRIVATE_DEFINITION_UWP() 0
#define BMBF_PLATFORM_PRIVATE_DEFINITION_WIN32() 0
#define BMBF_PLATFORM(X) BMBF_PLATFORM_PRIVATE_DEFINITION_##X()

#if defined(_WIN32)
    #undef BMBF_PLATFORM_PRIVATE_DEFINITION_MICROSOFT
    #define BMBF_PLATFORM_PRIVATE_DEFINITION_MICROSOFT() 1

    #if defined(WINAPI_FAMILY) && !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
        #undef BMBF_PLATFORM_PRIVATE_DEFINITION_UWP
        #define BMBF_PLATFORM_PRIVATE_DEFINITION_UWP() 1
    #else
        #undef BMBF_PLATFORM_PRIVATE_DEFINITION_WIN32
        #define BMBF_PLATFORM_PRIVATE_DEFINITION_WIN32() 1
    #endif
#elif defined(__APPLE__)
    #include "TargetConditionals.h"

    #undef BMBF_PLATFORM_PRIVATE_DEFINITION_POSIX
    #define BMBF_PLATFORM_PRIVATE_DEFINITION_POSIX() 1
    #undef BMBF_PLATFORM_PRIVATE_DEFINITION_APPLE
    #define BMBF_PLATFORM_PRIVATE_DEFINITION_APPLE() 1

    #if TARGET_OS_SIMULATOR
        #undef BMBF_PLATFORM_PRIVATE_DEFINITION_IOS
        #define BMBF_PLATFORM_PRIVATE_DEFINITION_IOS() 1

        #undef BMBF_PLATFORM_PRIVATE_DEFINITION_IOS_SIMULATOR
        #define BMBF_PLATFORM_PRIVATE_DEFINITION_IOS_SIMULATOR() 1
    #elif TARGET_OS_IPHONE
        #undef BMBF_PLATFORM_PRIVATE_DEFINITION_IOS
        #define BMBF_PLATFORM_PRIVATE_DEFINITION_IOS() 1
    #elif TARGET_OS_MAC
        #undef BMBF_PLATFORM_PRIVATE_DEFINITION_MACOS
        #define BMBF_PLATFORM_PRIVATE_DEFINITION_MACOS() 1
    #endif
#elif defined(__LINUX__)
    #undef BMBF_PLATFORM_PRIVATE_DEFINITION_POSIX
    #define BMBF_PLATFORM_PRIVATE_DEFINITION_POSIX() 1

    #undef BMBF_PLATFORM_PRIVATE_DEFINITION_LINUX
    #define BMBF_PLATFORM_PRIVATE_DEFINITION_LINUX() 1
#endif

/**************************************************************************************************/

/*

    FEATURE macros define what application-level features should be included in the compilation. The
    set of features is always derived from a combination of the target product and platform(s).
    There are any number of features defined per translation unit.

*/

#define BMBF_FEATURE_PRIVATE_DEFINITION_APPLE_DESKTOP() \
    BMBF_PLATFORM(APPLE) && BMBF_PRODUCT(DESKTOP)
#define BMBF_FEATURE_PRIVATE_DEFINITION_MICROSOFT_MOBILE() \
    BMBF_PLATFORM(MICROSOFT) && BMBF_PRODUCT(MOBILE)
#define BMBF_FEATURE_PRIVATE_DEFINITION_EMBEDDED_LINUX() \
    BMBF_PLATFORM(LINUX) && BMBF_PRODUCT(EMBEDDED)
#define BMBF_FEATURE_PRIVATE_DEFINITION_METAL() \
    BMBF_PLATFORM(APPLE) && !BMBF_PLATFORM(IOS_SIMULATOR)
#define BMBF_FEATURE(X) BMBF_FEATURE_PRIVATE_DEFINITION_##X()

/**************************************************************************************************/

int main(int argc, char** argv) {
#if BMBF_PRODUCT(DESKTOP)
    std::cout << "Hello, Desktop!\n";
#endif
#if BMBF_PRODUCT(MOBILE)
  std::cout << "Hello, Mobile!\n";
#endif
#if BMBF_PRODUCT(EMBEDDED)
    std::cout << "Hello, Embedded!\n";
#endif

#if BMBF_PLATFORM(APPLE)
    std::cout << "Hello, Apple!\n";
#endif
#if BMBF_PLATFORM(IOS)
  std::cout << "Hello, iOS!\n";
#endif
#if BMBF_PLATFORM(IOS_SIMULATOR)
  std::cout << "Hello, iOS Simulator!\n";
#endif
#if BMBF_PLATFORM(LINUX)
  std::cout << "Hello, Linux!\n";
#endif
#if BMBF_PLATFORM(MACOS)
  std::cout << "Hello, macOS!\n";
#endif
#if BMBF_PLATFORM(MICROSOFT)
  std::cout << "Hello, Microsoft!\n";
#endif
#if BMBF_PLATFORM(POSIX)
  std::cout << "Hello, POSIX!\n";
#endif
#if BMBF_PLATFORM(UWP)
  std::cout << "Hello, UWP!\n";
#endif
#if BMBF_PLATFORM(WIN32)
    std::cout << "Hello, Win32!\n";
#endif

#if BMBF_FEATURE(METAL)
    std::cout << "Hello, Metal feature!\n";
#endif
#if BMBF_FEATURE(APPLE_DESKTOP)
    std::cout << "Hello, Apple desktop feature!\n";
#endif
#if BMBF_FEATURE(MICROSOFT_MOBILE)
    std::cout << "Hello, Microsoft mobile feature!\n";
#endif
#if BMBF_FEATURE(EMBEDDED_LINUX)
    std::cout << "Hello, embedded Linux feature!\n";
#endif
}
