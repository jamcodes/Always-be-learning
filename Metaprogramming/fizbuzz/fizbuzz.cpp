#include <iostream>
#include <tuple>
#include <type_traits>


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// FizzBuzz:
// - for every number divisible by 3 return 'Fizz',
// - for numbers divisible by 5 return 'Buzz'
// - for numbers divisible by 3 and 5 (dibisible by 15) return 'FizzBuzz'
// - otherwise return the number itself


// C++11 / C++14 SFINAE solution
// ------------------------------------------------------------------------------------------------
// not divisible by either 3 or 5 -> return the number itself
// template<std::size_t I >
// constexpr decltype(auto) fizzbuzz_number(std::index_sequence<I>,
//         std::enable_if_t<(I % 3 != 0 && I % 5 != 0), void*> = nullptr) noexcept
// {
//     return std::make_tuple(I);
// }

// // divisible by 3 -> return "Fizz"
// template<std::size_t I>
// constexpr decltype(auto) fizzbuzz_number(std::index_sequence<I>,
//         std::enable_if_t<I % 3 == 0 && I % 5 != 0, void*> = nullptr) noexcept
// {
//     return std::make_tuple("Fizz");
// }

// // divisible by 5 -> return "Buzz"
// template<std::size_t I>
// constexpr decltype(auto) fizzbuzz_number(std::index_sequence<I>,
//         std::enable_if_t<I % 3 != 0 && I % 5 == 0, void*> = nullptr) noexcept
// {
//     return std::make_tuple("Buzz");
// }

// // divisible by 3 and 5 (so divisible by 15) -> return "FizzBuzz"
// template<std::size_t I>
// constexpr decltype(auto) fizzbuzz_number(std::index_sequence<I>,
//         std::enable_if_t<I % 15 == 0, void*> = nullptr) noexcept
// {
//     return std::make_tuple("FizzBuzz");
// }
// ------------------------------------------------------------------------------------------------

// C++17 -> if constexpr solution
// ------------------------------------------------------------------------------------------------
template<std::size_t I>
constexpr decltype(auto) fizzbuzz_number(std::index_sequence<I>) noexcept
{
    if constexpr (I % 3 == 0 && I % 5 != 0) {
        return std::make_tuple("Fizz");
    }
    else if constexpr (I % 3 != 0 && I % 5 == 0) {
        return std::make_tuple("Buzz");
    }
    else if constexpr (I % 15 == 0) {
        return std::make_tuple("FizzBuzz");
    }
    else { // (I % 3 != 0 && I % 5 != 0)
        return std::make_tuple(I);
    }
}
// ------------------------------------------------------------------------------------------------

// Declaration
// template<std::size_t... Is>
// constexpr auto fizzbuzz(std::index_sequence<Is...>) noexcept;

// // base case
// template<>
// constexpr auto fizzbuzz(std::index_sequence<>) noexcept
// {
//     return std::tuple<>{};
// }

// // recursive definition
// template<std::size_t I, std::size_t... Is>
// constexpr auto fizzbuzz(std::index_sequence<I, Is...>) noexcept
// {
//     return std::tuple_cat(
//         fizzbuzz_number(std::index_sequence<I>{}),
//         fizzbuzz(std::index_sequence<Is...>{})
//     );
// }

// or just...
template<std::size_t... Is>
constexpr auto fizzbuzz(std::index_sequence<Is...>) noexcept
{
    return std::tuple_cat(fizzbuzz_number(std::index_sequence<Is>{})...);
}
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------


// pretty-print a tuple
// ------------------------------------------------------------------------------------------------
template<typename CharT, typename Traits, typename Tuple, std::size_t... Is>
void print_tuple_impl(std::basic_ostream<CharT, Traits>& os,
                      const Tuple& tuple,
                      std::index_sequence<Is...>)
{
    ((os << (Is == 0 ? "" : ", ") << std::get<Is>(tuple)), ...);
}

template<typename CharT, typename Traits, typename... Args>
decltype(auto) operator<<(std::basic_ostream<CharT, Traits>& os, const std::tuple<Args...>& tuple)
{
    os << "(";
    print_tuple_impl(os, tuple, std::index_sequence_for<Args...>{});
    return os << ")";
}
// ------------------------------------------------------------------------------------------------

// Alternative pretty-print solution using `apply`
// ------------------------------------------------------------------------------------------------
// template<typename T>
// void print(std::ostream& os, const T& t)
// {
//     os << t; // no separator after last or only element
// }

// template<typename T, typename... Ts>
// void print(std::ostream& os, const T& t, const Ts&... ts)
// {
//     os << t << ", ";
//     print(os, ts...);
// }

template<typename F, typename Tuple, std::size_t... Is>
constexpr decltype(auto) apply_impl(F&& f, Tuple&& tuple, std::index_sequence<Is...>) noexcept
{
    return std::forward<F>(f)( std::get<Is>(std::forward<Tuple>(tuple))... );
}

template<typename F, typename Tuple>
constexpr decltype(auto) apply(F&& f, Tuple&& tuple) noexcept
{
    using Indices = std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>;
    return apply_impl(std::forward<F>(f), std::forward<Tuple>(tuple), Indices{});
}

// template<typename... Ts>
// std::ostream& operator<<(std::ostream& os, const std::tuple<Ts...>& tuple)
// {
//     const auto printer = [&os=os](auto&&... ts){print(os, ts...);};
//     os << "(";
//     myapply(printer, tuple);
//     return os << ")";
// }
// ------------------------------------------------------------------------------------------------


// evaluate at compiletime
constexpr auto expected = std::make_tuple(
        1, 2, "Fizz", 4, "Buzz", "Fizz", 7, 8,
        "Fizz", "Buzz", 11, "Fizz", 13, 14, "FizzBuzz", 16);
constexpr auto result = fizzbuzz(std::index_sequence<1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16>{});
static_assert(expected == result, "Tuples are not equal");

int main()
{
    std::cout << "expected: " << expected << "\n";
    std::cout << "result: " << result << "\n";

}
