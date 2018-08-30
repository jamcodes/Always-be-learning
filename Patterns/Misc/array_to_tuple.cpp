#include <iostream>
#include <array>
#include <tuple>
#include <type_traits>


// Convert array into a tuple
template<typename Array, std::size_t... I>
constexpr auto a2t_impl(const Array& a, std::index_sequence<I...>)
{
    return std::make_tuple(a[I]...);
}
 
template<typename T, std::size_t N, typename Indices = std::make_index_sequence<N>>
constexpr auto a2t(const T(&arr)[N])
{
    return a2t_impl(arr, Indices{});
}

template<typename T, std::size_t N, typename Indices = std::make_index_sequence<N>>
constexpr auto a2t(const std::array<T,N>& a)
{
    return a2t_impl(a, Indices{});
}

int main()
{
    constexpr const auto a1 = std::array<int, 4>{1,2,3,4};
    constexpr int a2[4] = {1,2,3,4};

    constexpr auto t1 = a2t(a1);
    constexpr auto t2 = a2t(a2);
    static_assert(t1 == t2, "Tuples are different");
}
