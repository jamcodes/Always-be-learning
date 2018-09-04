#include <iostream>
#include <type_traits>
#include <utility>

using std::size_t;


template<size_t Index, typename T>
struct element { using type = T; };

template<size_t I, typename T>
using element_t = typename element<I,T>::type;

template<typename Indices, typename... Ts>  // Indices will match a std::index_sequence
struct tuple_impl { };

// inherit from element<0, T0>, element<1, T1>, element<2, T2> ...
template<size_t... Is, typename... Ts>
struct tuple_impl<std::index_sequence<Is...>, Ts...> : element<Is, Ts>... { };

template<typename... Ts>
struct tuple : tuple_impl<std::make_index_sequence<sizeof...(Ts)>, Ts...> { };

// now all we need to do to access an element is static_cast to one of the element types
// like element<1, double>. To do that by specifying only the index we need to pattern match again
template<size_t I, typename T>
element_t<I, T> get(const element<I, T>&) noexcept;
// We don't actually need to provide a definition since we only need it for deduction purposes

// `at` finds the return type of get
template<size_t I, typename Tuple>
using at = decltype( get<I>(std::declval<Tuple>()) );


int main()
{
    using mytup = tuple<int, char, double>;

    using elem_0 = at<0, mytup>;
    static_assert(std::is_same_v<elem_0, int>, "int expected");

    using elem_1 = at<1, mytup>;
    static_assert(std::is_same_v<elem_1, char>, "char expected");

    using elem_2 = at<2, mytup>;
    static_assert(std::is_same_v<elem_2, double>, "double expected");

}
