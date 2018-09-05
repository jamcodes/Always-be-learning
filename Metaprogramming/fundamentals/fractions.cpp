#include <iostream>
#include <type_traits>
#include <utility>

using std::size_t;


template<size_t N, size_t D>
struct Fraction {
    static constexpr inline auto Num{N};
    static constexpr inline auto Den{D};
};

template<size_t X, typename F>
struct ScalarMultply {
    using value = Fraction<F::Num * X, F::Den * X>;
};

template<size_t X, size_t Y>
struct MCD {
    static constexpr inline auto value{MCD<Y, X % Y>::value};
};

template<size_t X>
struct MCD<X,0> {
    static constexpr inline auto value{X};
};

template<typename F>
struct Simplify {
    static constexpr inline auto mcd = MCD<F::Num, F::Den>::value;
    using value = Fraction<F::Num / mcd, F::Den / mcd>;
};

template<typename F1, typename F2>
struct AsCommonBase {
    using X = typename ScalarMultply<F2::Den, F1>::value;
    using Y = typename ScalarMultply<F1::Den, F2>::value;
};

template<typename X, typename Y>
struct Sum {
    using B = AsCommonBase<X,Y>;
    static constexpr inline auto Num = B::X::Num + B::Y::Num;
    static constexpr inline auto Den = B::X::Den;  // == B::Y::Den
    using value = typename Simplify<Fraction<Num, Den>>::value;
};


// calculate e
constexpr auto fact(size_t n) noexcept
{
    auto result = size_t{1};
    while(n) { result *= n--; }
    return result;
}

template<size_t N>
struct E {
    // e = S(1/n!) = 1/0! + 1/1! + 1/2! + ... + 1/n!
    static constexpr inline size_t Den{fact(N)};
    using term = Fraction<1, Den>;
    using next_term = typename E<N-1>::value;
    using value = typename Sum<term, next_term>::value;
};

template<>
struct E<0> {
    using value = Fraction<1,1>;
};


int main()
{
    using e_fraction = E<8>::value;
    std::cout << "e = " << e_fraction::Num << "/" << e_fraction::Den << "\n";
    std::cout << "e = " << (static_cast<double>(e_fraction::Num) / e_fraction::Den) << "\n";
}
