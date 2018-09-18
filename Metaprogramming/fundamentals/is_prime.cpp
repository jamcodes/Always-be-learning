#include <iostream>
#include <type_traits>


// Computing if a number is prime using template partial specialization and recursion
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
template<unsigned I, unsigned D>
struct is_prime_impl {
    static constexpr inline bool value = (I%D != 0) && is_prime_impl<I,D-1>::value;
};

template<unsigned I>
struct is_prime_impl<I,2> {
    static constexpr inline bool value = (I%2 != 0);
};

template<unsigned I>
struct is_prime {
    static constexpr bool value = is_prime_impl<I,I/2>::value;
};

template<>
struct is_prime<0> { static constexpr inline bool value = false; };
template<>
struct is_prime<1> { static constexpr inline bool value = false; };
template<>
struct is_prime<2> { static constexpr inline bool value = true; };
template<>
struct is_prime<3> { static constexpr inline bool value = true; };

template<unsigned I>
static constexpr inline auto is_prime_v = is_prime<I>::value;


static_assert(is_prime_v<1> == false, "is_prime<1> test case failed");
static_assert(is_prime_v<0> == false, "is_prime<0> test case failed");
static_assert(is_prime_v<2>, "is_prime<2> test case failed");
static_assert(is_prime_v<3>, "is_prime<3> test case failed");
static_assert(is_prime_v<11>, "is_prime<11> test case failed");
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

// Computing if a number is prime using constexpr
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// C++11 - only return statements and recursion constexpr
constexpr bool is_prime_cpp11_impl(unsigned p, unsigned d) noexcept // p: num to check, d: divisor
{
    return d!=2 ? (p%d != 0) && is_prime_cpp11_impl(p, d-1)
                : (p%2 != 0);
}

constexpr bool is_prime_cpp11(unsigned p) noexcept
{
    return p < 4 ? !(p<2)                       // special cases - 0, 1, 2, 3
                 : is_prime_cpp11_impl(p,p/2);  // start recursion with p/2
}

constexpr bool is_prime_cpp14(unsigned p) noexcept
{
    for (auto d = 2u; d <= p/2; ++d) {
        if (p % 2 == 0) {
            return false;
        }
    }
    return p > 1;   // special cases 0, 1
}

static_assert(is_prime_cpp11(0) == false, "");
static_assert(is_prime_cpp11(1) == false, "");
static_assert(is_prime_cpp11(2), "");
static_assert(is_prime_cpp11(3), "");
static_assert(is_prime_cpp11(4) == false, "");
static_assert(is_prime_cpp11(11), "");

static_assert(is_prime_cpp14(0) == false, "");
static_assert(is_prime_cpp14(1) == false, "");
static_assert(is_prime_cpp14(2), "");
static_assert(is_prime_cpp14(3), "");
static_assert(is_prime_cpp14(4) == false, "");
static_assert(is_prime_cpp14(11), "");
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

int main()
{
}
