#include <iostream>
#include <string>
#include <type_traits>

// Controlling overload resolution can be necessary when implementing overload sets
// or when certain argument categories can allow error-prone use cases.

// # Preventing implicit conversions
// Conversions for specific types or entire type categories can be disabled using `=delete`
void foo(int) { std::cout << "int\n"; }
void foo(float) = delete;
void foo(double) = delete;
void foo(long double) = delete;

// Disable the conversion for all floating point types:
template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
void foo(T) = delete;

// Prohibiting temporaries
// Either for safety or performance reasons we may want to disallow temporaries to be used with
// an overload set. For example if a function stores a pointer or reference to the passed argument
// and the argument is a temporary the stored pointer will dangle.
void bar(std::string const& obj)
{
    // store pointer to obj somewhere outside the function, or return it
}
// The above function can be made safer if temporaries are disallowed:
void bar(std::string const&&) = delete;
// overload on `const&&` will match both `&&` and `const&&` arguments - disallowing all temporaries.


// # Improve error messages for large overload sets, by declaring a deleted or static_asserted
// fallback overload that's always the worst match in the overload set.
template<typename T, std::size_t N>
constexpr auto array_size(T(&)[N]) noexcept
{
    return N;
}

template<typename T>
constexpr auto array_size(T) noexcept = delete;

// or
template<typename T>
constexpr auto array_size(T) noexcept
{
    static_assert(sizeof(T) == 0, "array_size() may be used only on array types");
    return 0;
}

int main()
{

}
