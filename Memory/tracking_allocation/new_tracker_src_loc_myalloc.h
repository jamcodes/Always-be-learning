#pragma once

#include <cstdio>   // for printf()
#include <cstdlib>  // for malloc() and aligned_alloc()
#include <new>      // for std::align_val_t
#ifdef _MSC_VER
#include <malloc.h>  // for _aligned_malloc() and _aligned_free()
#endif

#include <algorithm>
#include <array>
#include <limits>

#include "my_allocator.h"

namespace jam {

struct source_location final {
    const char* file;
    unsigned line;
};

static_assert(std::is_trivial_v<source_location> && std::is_standard_layout_v<source_location>,
              "source_location is expected to be trivial");

}  // namespace jam

namespace {
constexpr jam::source_location null_source_location{nullptr, std::numeric_limits<unsigned>{}.max()};
}  // namespace

#define JAM_SRC_LOC() \
    jam::source_location { __FILE__, static_cast<unsigned>(__LINE__) }

// CAUTION! This file defines a macro which replaces all operator new calls.
// See the very bottom of the file.
// ===========================================================================
// #define new new(JAM_SRC_LOC())
// ===========================================================================

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
    _aligned_free(p);                     // MSYS, MinGW
#else
    std::free(p);                            // C++17 API
#endif
}

struct NewTracker {
    // Use a vector with an allocator that doesn't use new/delete, to still allow for allocation
    // while tracing memory allocations that do use new/delete
    template <typename T>
    using trace_vector = std::vector<T, MyAlloc<T>>;

    static inline trace_vector<const void*> allocs;
    static inline trace_vector<jam::source_location> traces;
    static inline int num_allocs{0};  // num allocations calls
    static inline int num_delete{0};
    static inline size_t total_allocated{0};  // bytes allocated so far
    // static inline size_t total_deleted{0};  // bytes deleted so far
    static inline bool do_trace{false};  // tracing enabled
    // static inline bool in_new{false};    // don't track output inside new overloads

    static void reset() noexcept
    {
        allocs.clear();
        traces.clear();
        num_allocs = 0;
        num_delete = 0;
        total_allocated = 0;
    }

    static void trace(bool b) noexcept { do_trace = b; }

    [[nodiscard]] static void* allocate(std::size_t size, std::size_t align,
                                        jam::source_location loc, const char* call) noexcept
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
            printf_s("|  total: %zu bytes  ", total_allocated);
            printf_s("|  file: %s, line: %d", loc.file, loc.line);
            puts("");
        }
        allocs.push_back(std::move(p));
        traces.push_back(std::move(loc));
        return p;
    }

    static void deallocate(void* p, std::size_t size, std::size_t align, const char* call) noexcept
    {
        ++num_delete;
        // total_deleted += size;

        auto const idx =
            std::distance(allocs.cbegin(), std::find(allocs.cbegin(), allocs.cend(), p));
        allocs.at(idx) = nullptr;
        traces[idx] = null_source_location;

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
                auto const t{traces[i]};
                printf_s("Allocation #%d, @ %p | file: %s, line: %d\n", i, p, t.file, t.line);
            }
        }
    }
};

// Since we're attempting to replace all `new` calls with `new(jam::source_location{...})`
// we don't overload the default new and new[] - instead overload the matching signatures
#if 0
[[nodiscard]] void* operator new(std::size_t size)
{  //
    return NewTracker::allocate(size, 0, "::new");
}

[[nodiscard]] void* operator new[](std::size_t size)
{
    return NewTracker::allocate(size, 0, "::new[]");
}
#endif

[[nodiscard]] void* operator new(std::size_t size, jam::source_location loc)
{  //
    return NewTracker::allocate(size, 0, loc, "::new");
}

[[nodiscard]] void* operator new[](std::size_t size, jam::source_location loc)
{
    return NewTracker::allocate(size, 0, loc, "::new[]");
}

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

// I can't see a way right now to achieve this for custom-aligned data
// or other placement-new-like calls
#if 0 // NEW_TRACKER_ALIGNED_OVERLOADS
// aligned new
[[nodiscard]] void* operator new(std::size_t size, std::align_val_t align)
{
    return NewTracker::allocate(size, static_cast<size_t>(align), "::new (size_t, align_val_t)");
}

[[nodiscard]] void* operator new[](std::size_t size, std::align_val_t align)
{
    return NewTracker::allocate(size, static_cast<size_t>(align), "::new[] (size_t, align_val_t)");
}

// aligned delete
void operator delete(void* p, std::align_val_t a) noexcept
{
    NewTracker::deallocate(p, 0, static_cast<size_t>(a), "delete(void*, align_val_t)");
}
void operator delete(void* p, std::size_t s, std::align_val_t a) noexcept
{
    NewTracker::deallocate(p, s, static_cast<size_t>(a), "delete(void*, size_t, align_val_t)");
}
void operator delete[](void* p, std::align_val_t a) noexcept
{
    NewTracker::deallocate(p, 0, static_cast<size_t>(a), "delete[] (void*, align_val_t)");
}
void operator delete[](void* p, std::size_t s, std::align_val_t a) noexcept
{
    NewTracker::deallocate(p, s, static_cast<size_t>(a), "delete[] (void*, size_t, align_val_t)");
}
#endif

// Redirect all `new` calls to our custom, placement-new-overload
// ===========================================================================
#define new new(JAM_SRC_LOC())
// ===========================================================================
