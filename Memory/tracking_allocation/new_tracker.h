#pragma once

#include <cstdio>   // for printf()
#include <cstdlib>  // for malloc() and aligned_alloc()
#include <new>      // for std::align_val_t
#ifdef _MSC_VER
#include <malloc.h>  // for _aligned_malloc() and _aligned_free()
#endif

#include <algorithm>
#include <array>

[[nodiscard]] inline void* my_aligned_alloc(std::size_t size, std::size_t align) noexcept
{
#ifdef _MSC_VER
    return _aligned_malloc(size, align);  // Windows API
#elif defined(__MINGW32__) || defined(__MINGW64__)
    return _aligned_malloc(size, align);  // MSYS, MinGW
#else
    return std::aligned_alloc(align, size);  // C++17 API
#endif
}

inline void my_aligned_free(void* p) noexcept
{
#ifdef _MSC_VER
    _aligned_free(p);  // Windows API
#elif defined(__MINGW32__) || defined(__MINGW64__)
    _aligned_free(p);   // MSYS, MinGW
#else
    std::free(p);   // C++17 API
#endif
}

struct NewTracker {
    static inline std::array<const void*, 100> allocs{nullptr};
    static inline int num_allocs{0};  // num allocations calls
    static inline int num_delete{0};
    static inline size_t total_allocated{0};  // bytes allocated so far
    // static inline size_t total_deleted{0};  // bytes deleted so far
    static inline bool do_trace{false};  // tracing enabled
    // static inline bool in_new{false};    // don't track output inside new overloads

    static void reset() noexcept
    {
        allocs.fill(nullptr);
        num_allocs = 0;
        num_delete = 0;
        total_allocated = 0;
    }

    static void trace(bool b) noexcept
    {
        do_trace = b;
    }

    [[nodiscard]] static void* allocate(std::size_t size, std::size_t align,
                                        const char* call) noexcept
    {
        ++num_allocs;
        total_allocated += size;

        void* const p = [&]() noexcept {
            if (align == 0) {
                return std::malloc(size);
            }
            else {
                return my_aligned_alloc(size, align);
            }
        }();
        if (do_trace) {
            printf_s("%s   \t#%d: %zu bytes, @ %p  ", call, num_allocs, size, p);
            if (align > 0) {
                printf_s("|  %zu-byte aligned  ", align);
            }
            else {
                printf_s("|  def-aligned  ");
            }
            printf_s("|  total: %zu bytes\n", total_allocated);
        }
        allocs.at(num_allocs) = p;
        return p;
    }

    static void deallocate(void* p, std::size_t size, std::size_t align, const char* call) noexcept
    {
        ++num_delete;
        // total_deleted += size;

        auto const idx = std::distance(allocs.cbegin(), std::find(allocs.cbegin(), allocs.cend(), p));
        allocs.at(idx) = nullptr;

        if (align == 0) {
            std::free(p);
        }
        else {
            my_aligned_free(p);
        }
        if (do_trace) {
            printf_s("%s   \t#%d: %zu bytes, @ %p  ", call, num_delete, size, p);
            if (align > 0) {
                printf_s("|  %zu-byte aligned  ", align);
            }
            else {
                printf_s("|  def-aligned  ");
            }
            // printf_s("|  total: %zu bytes\n", total_deallocated);
            puts("");
        }
    }

    static void status() noexcept
    {
        printf_s("Allocations:   %d => %zu bytes\n", num_allocs, total_allocated);
        printf_s("Deallocations: %d\n", num_delete);
        puts("\nNon-deallocated memory");
        for (auto i{0}; i != allocs.size(); ++i) {
            auto const p = allocs[i];
            if (p != nullptr) {
                printf_s("Allocation #%d, @ %p\n", i, p);
            }
        }
    }
};

[[nodiscard]] void* operator new(std::size_t size)
{  //
    return NewTracker::allocate(size, 0, "::new");
}

[[nodiscard]] void* operator new[](std::size_t size)
{
    return NewTracker::allocate(size, 0, "::new[]");
}

// Ignore aligned overloads for now

// [[nodiscard]] void* operator new(std::size_t size, std::align_val_t align)
// {
//     return NewTracker::allocate(size, static_cast<size_t>(align), "::new (size_t, align_val_t)");
// }

// [[nodiscard]] void* operator new[](std::size_t size, std::align_val_t align)
// {
//     return NewTracker::allocate(size, static_cast<size_t>(align), "::new[] (size_t, align_val_t)");
// }

// ensure deallocations match:
void operator delete(void* p) noexcept
{  //
    NewTracker::deallocate(p, 0, 0, "delete(void*)");
}
void operator delete(void* p, std::size_t s) noexcept
{
    NewTracker::deallocate(p, s, 0, "delete(void*, size_t)");
}
void operator delete[](void* p) noexcept
{  //
    NewTracker::deallocate(p, 0, 0, "delete[] (void*)");
}
void operator delete[](void* p, std::size_t s) noexcept
{
    NewTracker::deallocate(p, s, 0, "delete[] (void*, size_t)");
}

// Ignore aligned overloads for now
// void operator delete(void* p, std::align_val_t a) noexcept
// {
//     NewTracker::deallocate(p, 0, static_cast<size_t>(a), "delete(void*, align_val_t)");
// }
// void operator delete(void* p, std::size_t s, std::align_val_t a) noexcept
// {
//     NewTracker::deallocate(p, s, static_cast<size_t>(a), "delete(void*, size_t, align_val_t)");
// }
// void operator delete[](void* p, std::align_val_t a) noexcept
// {
//     NewTracker::deallocate(p, 0, static_cast<size_t>(a), "delete[] (void*, align_val_t)");
// }
// void operator delete[](void* p, std::size_t s, std::align_val_t a) noexcept
// {
//     NewTracker::deallocate(p, s, static_cast<size_t>(a), "delete[] (void*, size_t, align_val_t)");
// }