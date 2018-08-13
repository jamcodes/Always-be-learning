#include <iostream>
#include <vector>
#include <type_traits>


// Implement a type trait that returns true if std::vector is passed to it
template<typename T>
struct is_stdvector : std::false_type { };

template<typename... Args>
struct is_stdvector<std::vector<Args...>> : std::true_type { };

template<typename T>
using is_stdvector_t = typename is_stdvector<std::decay_t<T>>::type;

template<typename T>
void foo_impl(T&& /* v */, std::false_type /*unused*/) noexcept
{
    std::cout << "non-std::vector implementation\n";
}

template<typename T>
void foo_impl(T&& /* v */, std::true_type /*unused*/) noexcept
{
    std::cout << "std::vector-optimized implementation\n";
}

template<typename T>
void foo(T&& v) noexcept
{
    foo_impl(std::forward<T>(v), is_stdvector_t<T>{});
}


int main()
{
    auto vec = std::vector<int>{};
    foo(vec);
    foo(42);
}
