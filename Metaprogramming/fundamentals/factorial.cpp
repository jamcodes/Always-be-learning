#include <iostream>
#include <type_traits>
#include <utility>

using std::size_t;

// classical template specialization example using specialization
template<size_t N>
struct factorial { static constexpr inline size_t value{N * factorial<N-1>::value}; };

template<>
struct factorial<0> { static constexpr inline size_t value{1}; };

// could actually use a function template - only full specializations
template<size_t N>
constexpr size_t factorial_func() noexcept { return N * factorial_func<N-1>(); }
template<>
constexpr size_t factorial_func<0>() noexcept { return 1; }

// or a regular constexpr function
constexpr auto factorial_constexpr(size_t n) noexcept
{
    auto result = size_t{1};
    while(n) {
        result *= n--;
    }
    return result;
}

int main()
{
    constexpr auto fstruct_result = factorial<5>::value;
    std::cout << "factorial<5>::value = " << fstruct_result << "\n";
    constexpr auto ffunc_result = factorial_func<5>();
    std::cout << "factorial_func<5>() = " << ffunc_result << "\n";
    constexpr auto fcxrp_result = factorial_constexpr(5);
    std::cout << "factorial_constexpr(5) = " << fcxrp_result << "\n";
}
