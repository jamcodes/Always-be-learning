#include <iostream>
#include <string>
#include <type_traits>

using namespace std::string_literals;

template<typename T, typename = void>
struct data_processor
{
    double operator()(const T& v1, const T& v2, const T& v3)
    {
        std::cout << "\nGeneric version: "
            << __PRETTY_FUNCTION__ << "\n";
        std::cout << "\t" << v1 << ", " << v2 << ", " << v3 << "\n";
        return 1.0;
    }
};

// spcializations
template<typename T>
struct data_processor<T, std::enable_if_t<
        std::is_integral_v<std::decay_t<T>>>>
{
    using fast_int_t = int;
    double operator()(fast_int_t v1, fast_int_t v2, fast_int_t v3)
    {
        std::cout << "\nint-optimized version " << __PRETTY_FUNCTION__ << "\n";
        std::cout << "\t" << v1 << ", " << v2 << ", " << v3 << "\n";
        return 2.0;
    }
};

template<typename T>
struct data_processor<T, std::enable_if_t<
        std::is_floating_point_v<std::decay_t<T>>>>
{
    double operator()(double v1, double v2, double v3)
    {
        std::cout << "\ndouble-optimized version " << __PRETTY_FUNCTION__ << "\n";
        std::cout << "\t" << v1 << ", " << v2 << ", " << v3 << "\n";
        return 3.0;
    }
};


template<typename T>
double example(T&& v1, T&& v2, T&& v3)
{
    auto proc = data_processor<T>{};
    return proc(std::forward<T>(v1), std::forward<T>(v2), std::forward<T>(v3));
}


int main()
{
    // int-optimized version
    example(1, 2, 3);
    auto s = short{42};
    example(s, s, s);

    // float optimized version
    example(3.14, 2.72, 4.14);
    auto f = float{3.33f};
    example(f, f, f);

    // primary template
    example("Hello"s, " specialization "s, "world"s);
}
