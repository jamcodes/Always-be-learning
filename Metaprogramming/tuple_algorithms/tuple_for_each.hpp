#if !defined(TUPLE_FOR_EACH_HEADER_GUARD_)
#define TUPLE_FOR_EACH_HEADER_GUARD_

#include <tuple>
#include <type_traits>
#include <utility>

template <typename Tuple, typename Function, int... Is>
constexpr void
tuple_for_each_impl(Tuple&& tuple, Function&& function, std::integer_sequence<int, Is...>) noexcept(
    (... && std::is_nothrow_invocable_v<Function, std::tuple_element_t<Is, std::decay_t<Tuple>>>))
{
    (..., std::forward<Function>(function)(std::get<Is>(std::forward<Tuple>(tuple))));
}

template <typename Tuple, typename Function>
constexpr void tuple_for_each(Tuple&& tuple, Function&& function)
// noexcept(std::is_nothrow_invocable_v<tuple_for_each_impl<Tuple, Function)
{
    using Indices = std::make_integer_sequence<int, std::tuple_size_v<std::decay_t<Tuple>>>;
    tuple_for_each_impl(std::forward<Tuple>(tuple), std::forward<Function>(function), Indices{});
}

namespace alt {

template <typename Tuple, typename Function>
constexpr void tuple_for_each_helper(Tuple&& /* tuple */, Function&& /* function */)
{
}

template <std::size_t I, std::size_t... Is, typename Tuple, typename Function>
constexpr void tuple_for_each_helper(Tuple&& tuple, Function&& function)
{
    std::forward<Function>(function)(std::get<I>(std::forward<Tuple>(tuple)));
    alt::tuple_for_each_helper<Is...>(std::forward<Tuple>(tuple), std::forward<Function>(function));
}

template <typename Tuple, typename Function, std::size_t... Is>
constexpr void tuple_for_each_impl(Tuple&& tuple, Function&& function, std::index_sequence<Is...>)
{
    alt::tuple_for_each_helper<Is...>(std::forward<Tuple>(tuple), std::forward<Function>(function));
}

template <typename Tuple, typename Function>
constexpr void tuple_for_each(Tuple&& tuple, Function&& function)
{
    using Indices = std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>;
    alt::tuple_for_each_impl(std::forward<Tuple>(tuple), std::forward<Function>(function),
                             Indices{});
}

}  // namespace alt

namespace alt2 {

template <typename Tuple, typename Function, std::size_t N>
struct tuple_for_each_helper {
    using self = alt2::tuple_for_each_helper<Tuple, Function, N>;

    template <std::size_t I>
    static constexpr void call(Tuple tuple, Function function)
    {
        std::forward<Function>(function)(std::get<I>(std::forward<Tuple>(tuple)));
        self::call<I + 1>(std::forward<Tuple>(tuple), std::forward<Function>(function));
    }

    template <>
    static constexpr void call<N>(Tuple, Function)
    {
    }
};

template <typename Tuple, typename Function>
constexpr void tuple_for_each(Tuple&& tuple, Function&& function)
{
    constexpr auto size = std::tuple_size_v<std::decay_t<Tuple>>;
    alt2::tuple_for_each_helper<Tuple, Function, size>::template call<0>(std::forward<Tuple>(tuple),
                                                             std::forward<Function>(function));
}

}  // namespace alt2

#endif  // TUPLE_FOR_EACH_HEADER_GUARD_
