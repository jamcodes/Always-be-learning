#pragma once

#include <cstddef>
#include <stdexcept>

template <class T>
class malloc_allocator {
public:
    using value_type = T ;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    template <class U>
    struct rebind {
        typedef malloc_allocator<U> other;
    };

    constexpr malloc_allocator() = default;
    constexpr malloc_allocator(const malloc_allocator&) = default;
    constexpr malloc_allocator(malloc_allocator&&) noexcept = default;
    malloc_allocator& operator=(malloc_allocator const&) = delete;
    malloc_allocator& operator=(malloc_allocator&&) = delete;
    template <class U>
    malloc_allocator(const malloc_allocator<U>&)
    {
    }
    template <class U>
    malloc_allocator(const malloc_allocator<U>&&)
    {
    }
    ~malloc_allocator() = default;

    pointer address(reference x) const noexcept { return &x; }
    const_pointer address(const_reference x) const noexcept { return x; }

    pointer allocate(size_type n, const_pointer = 0) const
    {
        void* p{std::malloc(n * sizeof(T))};
        if (!p) throw std::bad_alloc();
        return static_cast<pointer>(p);
    }

    void deallocate(pointer p, size_type) const noexcept { std::free(p); }

    size_type max_size() const noexcept { return static_cast<size_type>(-1) / sizeof(T); }

    void construct(pointer p, const value_type& x) const { new (p) value_type(x); }
    void destroy(pointer p) const noexcept { p->~value_type(); }
};

template <>
class malloc_allocator<void> {
    typedef void value_type;
    typedef void* pointer;
    typedef const void* const_pointer;

    template <class U>
    struct rebind {
        typedef malloc_allocator<U> other;
    };
};

template <class T>
inline bool operator==(const malloc_allocator<T>&, const malloc_allocator<T>&)
{
    return true;
}

template <class T>
inline bool operator!=(const malloc_allocator<T>&, const malloc_allocator<T>&)
{
    return false;
}
