#pragma once

#include <utility>
#include <type_traits>


template<typename From, typename To>
struct IsConvertibleImpl {
private:
    // test() - trying to call aux() for a value of type From
    static void aux(To);
    template<typename F, typename = decltype(aux(std::declval<F>()))>
    static std::true_type test(void*) noexcept;

    // test() fallback
    template<typename>
    static std::false_type test(...) noexcept;
public:
    using type = decltype(test<From>(nullptr));
};

template<typename From, typename To>
struct IsConvertibleT : IsConvertibleImpl<From, To>::type { };

// template<typename From, typename To>
// using IsConvertible = typename IsConvertibleT<From, To>::type;

template<typename From, typename To>
constexpr inline bool IsConvertible{IsConvertibleT<From, To>::value};
