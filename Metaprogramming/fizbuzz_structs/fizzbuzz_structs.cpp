#include <iostream>
#include <tuple>
#include <type_traits>


// FizzBuzz:
// - for every number divisible by 3 return 'Fizz',
// - for numbers divisible by 5 return 'Buzz'
// - for numbers divisible by 3 and 5 (dibisible by 15) return 'FizzBuzz'
// - otherwise return the number itself

// Solution using structs

template<std::size_t... Is>
struct Fizzbuzz;

template<std::size_t I, typename = void>
struct FizzbuzzSingle
{
    constexpr FizzbuzzSingle() noexcept = default;
    constexpr explicit FizzbuzzSingle(std::ostream& os)
        : os_{os} { }
// We could print the values on descruction
    // ~FizzbuzzSingle() noexcept { os_ << I << "\n"; }

// or use the call operator
    constexpr void operator()() noexcept
    {
        os_ << I << "\n";
    }

    std::ostream& os_{std::cout};
};

template<std::size_t I>
struct FizzbuzzSingle<I, std::enable_if_t<I % 3 == 0 && I % 5 != 0>>
{
    constexpr FizzbuzzSingle() noexcept = default;
    constexpr explicit FizzbuzzSingle(std::ostream& os)
        : os_{os} { }
    // ~FizzbuzzSingle() noexcept { os_ << "Fizz\n"; }

    constexpr void operator()() noexcept
    {
        os_ << "Fizz\n";
    }

    std::ostream& os_{std::cout};
};

template<std::size_t I>
struct FizzbuzzSingle<I, std::enable_if_t<I % 3 != 0 && I % 5 == 0>>
{
    constexpr FizzbuzzSingle() noexcept = default;
    constexpr explicit FizzbuzzSingle(std::ostream& os)
        : os_{os} { }
    // ~FizzbuzzSingle() noexcept { os_ << "Buzz\n"; }

    constexpr void operator()() noexcept
    {
        os_ << "Buzz\n";
    }

    std::ostream& os_{std::cout};
};

template<std::size_t I>
struct FizzbuzzSingle<I, std::enable_if_t<I % 3 == 0 && I % 5 == 0>>
{
    constexpr FizzbuzzSingle() noexcept = default;
    constexpr explicit FizzbuzzSingle(std::ostream& os)
        : os_{os} { }
    // ~FizzbuzzSingle() noexcept { os_ << "FizzBuzz\n"; }

    constexpr void operator()() noexcept
    {
        os_ << "FizzBuzz\n";
    }

    std::ostream& os_{std::cout};
};

template<>
struct Fizzbuzz<>
{
    constexpr Fizzbuzz() noexcept = default;
    constexpr explicit Fizzbuzz(std::ostream& os) noexcept { }

    constexpr void operator()() noexcept { }

// not needed - we're just passing the reference down the inheritance chain to FizzbuzzSingle
// in the partial specializations
    // std::ostream& os_{std::cout};
};

template<std::size_t I, std::size_t... Is>
struct Fizzbuzz<I,Is...> : Fizzbuzz<Is...>, FizzbuzzSingle<I>
{
    constexpr Fizzbuzz() noexcept = default;
    constexpr explicit Fizzbuzz(std::ostream& os) noexcept
        : Fizzbuzz<Is...>{os}, FizzbuzzSingle<I>{os} { }

    constexpr void operator()() noexcept
    {
        FizzbuzzSingle<I>::operator()();
        Fizzbuzz<Is...>::operator()();
    }
};

template<std::size_t... Is>
struct Fizzbuzz<0, Is...> : Fizzbuzz<Is...>
{
    // cannot inherit constructors from indirect base class
    // using Fizzbuzz<>:Fizzbuzz;
    constexpr Fizzbuzz() noexcept = default;
    constexpr explicit Fizzbuzz(std::ostream& os) noexcept
        : Fizzbuzz<Is...>{os} { }

    constexpr void operator()() noexcept
    {
        Fizzbuzz<Is...>::operator()();
    }
};


template<std::size_t... Is>
auto make_fizzbuzz(std::ostream& os, std::index_sequence<Is...>)
{
    return Fizzbuzz<Is...>{os};
}


int main()
{
    auto fb = make_fizzbuzz(std::cout, std::make_index_sequence<21>());
    fb();
}
