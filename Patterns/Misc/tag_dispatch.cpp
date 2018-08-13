#include <iostream>
#include <type_traits>


template<int I>
using Int_const_t = std::integral_constant<int, I>;

// Generic implementation
template<typename T, typename Tag>
T process_impl(T&& val, Tag /*unused*/)
{
    std::cout << "\nGeneric implementation: " << __PRETTY_FUNCTION__ << "\nval = " << val << "\n";
    return val;
}

// 1-byte optimized
template<typename T>
T process_impl(T val, Int_const_t<1> /*unused*/)
{
    std::cout << "\n1-byte optimized: " << __PRETTY_FUNCTION__ << "\nval = " << val << "\n";
    return val;
}

// 4-byte optimized
template<typename T>
T process_impl(T val, Int_const_t<4> /*unused*/)
{
    std::cout << "\n4-byte optimized: " << __PRETTY_FUNCTION__ << "\nval = " << val << "\n";
    return val;
}

// 8-byte optimized
template<typename T>
T process_impl(T val, Int_const_t<8> /*unused*/)
{
    std::cout << "\n8-byte optimized: " << __PRETTY_FUNCTION__ << "\nval = " << val << "\n";
    return val;
}

// tag-dispatch calls
template<typename T>
decltype(auto) process(T&& val)
{
    static_assert(std::is_pod_v<std::decay_t<T>>, "process called with non-pod type");
    return process_impl(std::forward<T>(val), Int_const_t<static_cast<int>(sizeof(T))>{});
}

struct Pod
{
    double a;
    int i;
    char c;
};

std::ostream& operator<<(std::ostream& os, Pod p)
{
    return os << "Pod{" << p.a << ", " << p.i << ", " << p.c << "}";
}

int main()
{
    process(true);
    process(42);
    process(3.14);
    process(Pod{2.72, 42, 'A'});
}
