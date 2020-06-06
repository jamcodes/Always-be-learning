#include <cstddef>  // for size_t
#include <cstdlib>  // for malloc and free

template <typename T>
class MyAlloc {
public:
    using value_type = T;
    // template <typename U>
    // MyAlloc(const MyAlloc<U>&) noexcept
    // { /* no state to copy */
    // }

    T* allocate(std::size_t num)
    {
        // return static_cast<T*>( ::operator new( num * sizeof(T) ) );
        return static_cast<T*>(std::malloc(num * sizeof(T)));
    }

    void deallocate(T* p, std::size_t)
    {
        // ::operator delete(p);
        std::free(p);
    }
};

// return that all specializations of this allocator are interchangable
template <typename T1, typename T2>
constexpr bool operator==(const MyAlloc<T1>&, const MyAlloc<T2>&) noexcept
{
    return true;
}

template <typename T1, typename T2>
constexpr bool operator!=(const MyAlloc<T1>&, const MyAlloc<T2>&) noexcept
{
    return false;
}
