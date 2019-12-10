#include <iostream>
#include <chrono>
#include <memory>

// When initializing an expression of type `X` from expression of type `Y` preffer implicit
// conversions whenever possible.
// An implicit conversion will fail at compiletime whenever it is not allowed - this may detect
// some errors at compiletime, which would otherwise fail at runtime (by causing hard to detect
// bugs). An explicit conversion on the other hand will succeed more often, also in cases when
// it may not be the intended behavior.

// Example 1 - std::chrono
// Explicit conversion
template<typename LhsDuration, typename RhsDuration>
auto avg_nanoseconds_explicit(LhsDuration lhs, RhsDuration rhs) noexcept
{
    using namespace std::chrono;
    const auto ns = nanoseconds{lhs + rhs}; // Explicit conversion
    return ns/2;
}

// Implicit conversion
template<typename LhsDuration, typename RhsDuration>
auto avg_nanoseconds_implicit(LhsDuration lhs, RhsDuration rhs) noexcept
{
    using namespace std::chrono;
    const nanoseconds ns = lhs + rhs;  // Implicit conversion
    return ns/2;
}

// Example 2 - std::shared_ptr
struct Base { };
struct Derived : public Base { };

// Explicit conversion
auto foo(std::shared_ptr<Derived> p)
{
    // lots of code
    auto bp = std::shared_ptr<Base>{p}; // Explicit conversion
    // more code ...
}

// foo() is later refactored to
auto foo(Derived* p)
{
    // lots of code
    auto bp = std::shared_ptr<Base>{p}; // Explicit conversion
    // more code ...
}
// foo() is now a runtime error
// the line `auto bp = std::shared_ptr<Base>{p} takes ownership of pointer `p`
// and potentially destroys it at scope exit. If it's not destroyed (ownership passed further)
// there are now two "owners" of pointer `p` - the original owner who passed pointer `p` into
// the function and the shared_pointer created within the function body.

// Implicit conversion
auto foo(std::shared_ptr<Derived> p)
{
    // lots of code
    std::shared_ptr<Base> bp = p; // Implicit conversion
    // more code ...
}

// foo() is later refactored to
auto foo(Derived* p)
{
    // lots of code
    std::shared_ptr<Base> bp = p; // Fails at compiletime!
    // more code ...
}

// In summary
// * Client code should preffer implicit conversions whenever possible - these are the
// conversions the type author consideres the safest,
// * Type authors should use either explicit or implicit - depending on the conversion that preservers
// the meaning of a type and that doesn't loose information,

int main()
{
    // Example 1
    // Explicit conversion
    using namespace std::chrono_literals;
    const auto x = avg_nanoseconds_explicit(2us, 1ms);   // 501000ns -> Good!
    const auto y = avg_nanoseconds_explicit(2, 1);       // 1ns -> Bad!
    // The second case adds the two integers and converts them to chrono::nanoseconds
    // explicitly - this will always compile successfully, but will produce unexpected results.

    // Implicit conversion
    const auto a = avg_nanoseconds_implicit(2us, 1ms);  // 501000ns -> Good!
    const auto b = avg_nanoseconds_implicit(2, 1);      // Fails to compile -> Good!
    // The implicit conversion from into to chrono::nanoseconds will fail at compiletime
    // with `error: no viable conversion from 'int' to 'nanoseconds'`
    // thus detecting a potentially hard to find runtime-bug at compiletime!
}
