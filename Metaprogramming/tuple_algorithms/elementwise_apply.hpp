#if !defined(ELEMENTWISE_APPLY_HEADER_GUARD_HPP_)
#define ELEMENTWISE_APPLY_HEADER_GUARD_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

/**
 * elementwise_apply invokes a given callable F with elements of the given Tuple pack passed as
 * arguments. The callable F is invoked N times where N is the number of elements in each tuple.
 * E.g for three Tuples of two elements:
 * function(std::get<0>(tuple_1), std::get<0>(tuple_2), ..., std::get<0>(tuple_N));
 * function(std::get<1>(tuple_1), std::get<1>(tuple_2), ..., std::get<1>(tuple_N));
 * function(std::get<M>(tuple_1), std::get<M>(tuple_2), ..., std::get<M>(tuple_N));
 */

template <typename...>
struct Typelist {
};
template <typename List>
struct TypelistFrontT;
template <typename T, typename... Ts>
struct TypelistFrontT<Typelist<T, Ts...>> {
    using type = T;
};
template <typename List>
using TypelistFront = typename TypelistFrontT<List>::type;

template <std::size_t I, typename Function, typename... Tuples>
constexpr void elementwise_apply_one(Function&& function, Tuples&&... tuples) noexcept
{
    std::forward<Function>(function)(std::get<I>(std::forward<Tuples>(tuples))...);
}

template <typename Function, std::size_t... Is, typename... Tuples>
constexpr void elementwise_apply_impl(Function&& function, std::index_sequence<Is...>,
                                      Tuples&&... tuples) noexcept
{
    (..., elementwise_apply_one<Is>(std::forward<Function>(function),
                                    std::forward<Tuples>(tuples)...));
}

template <typename Function, typename... Tuples>
constexpr void elementwise_apply(Function&& function, Tuples&&... tuples) noexcept
{
    constexpr auto size = std::tuple_size_v<std::decay_t<TypelistFront<Typelist<Tuples...>>>>;
    // TODO: Move the static_assert comparison to a dedicated function to allow elementwise_apply
    // to be called with only one tuple.
    static_assert((... && (size == std::tuple_size_v<std::decay_t<Tuples>>)),
                  "Tuples must have equal sizes");
    using Indices = std::make_index_sequence<size>;
    elementwise_apply_impl(std::forward<Function>(function), Indices{},
                           std::forward<Tuples>(tuples)...);
}

namespace alt {

template <typename Function, std::size_t... Is, typename... Tuples>
constexpr void elementwise_apply_impl(Function&& function, std::index_sequence<Is...>,
                                      Tuples&&... tuples) noexcept;

template <typename Function, typename... Tuples>
constexpr void elementwise_apply_impl(Function&&, std::index_sequence<>,
                                      Tuples&&...) noexcept
{
}

template <typename Function, std::size_t I, std::size_t... Is, typename... Tuples>
constexpr void elementwise_apply_impl(Function&& function, std::index_sequence<I, Is...>,
                                      Tuples&&... tuples) noexcept
{
    std::forward<Function>(function)(std::get<I>(std::forward<Tuples>(tuples))...);
    elementwise_apply_impl(std::forward<Function>(function), std::index_sequence<Is...>{},
                           std::forward<Tuples>(tuples)...);
}

template <typename Function, typename... Tuples>
constexpr void elementwise_apply(Function&& function, Tuples&&... tuples) noexcept
{
    constexpr auto size = std::tuple_size_v<std::decay_t<TypelistFront<Typelist<Tuples...>>>>;
    using Indices = std::make_index_sequence<size>;
    return elementwise_apply_impl(std::forward<Function>(function), Indices{},
                                  std::forward<Tuples>(tuples)...);
}
}  // namespace alt

namespace alt2 {

template <typename Indices>
struct elementwise_apply_impl;

template <>
struct elementwise_apply_impl<std::index_sequence<>> {
    template <typename Function, typename... Tuples>
    constexpr void operator()(Function&&, Tuples&&...) const noexcept
    {
    }
};

template <std::size_t I, std::size_t... Is>
struct elementwise_apply_impl<std::index_sequence<I, Is...>> {
    template <typename Function, typename... Tuples>
    constexpr void operator()(Function&& function, Tuples&&... tuples) const noexcept
    {
        std::forward<Function>(function)(std::get<I>(std::forward<Tuples>(tuples))...);
        elementwise_apply_impl<std::index_sequence<Is...>>{}(std::forward<Function>(function),
                                                             std::forward<Tuples>(tuples)...);
    }
};

template <typename Function, typename... Tuples>
constexpr void elementwise_apply(Function&& function, Tuples&&... tuples) noexcept
{
    constexpr auto size = std::tuple_size_v<std::decay_t<TypelistFront<Typelist<Tuples...>>>>;
    using Indices = std::make_index_sequence<size>;
    return elementwise_apply_impl<Indices>{}(std::forward<Function>(function),
                                             std::forward<Tuples>(tuples)...);
}
}  // namespace alt2

// simplification of elementwise_apply to just two tuples
template <typename Function, typename Tuple1, typename Tuple2, std::size_t... Is>
constexpr void apply_2_impl(Function&& function, Tuple1&& tuple1, Tuple2&& tuple2,
                            std::index_sequence<Is...>) noexcept
{
    (..., std::forward<Function>(function)(std::get<Is>(std::forward<Tuple1>(tuple1)),
                                           std::get<Is>(std::forward<Tuple2>(tuple2))));
}

template <typename Function, typename Tuple1, typename Tuple2>
constexpr void apply_2(Function&& function, Tuple1&& tuple1, Tuple2&& tuple2) noexcept
{
    constexpr auto size1 = std::tuple_size_v<std::decay_t<Tuple1>>;
    constexpr auto size2 = std::tuple_size_v<std::decay_t<Tuple2>>;
    static_assert(size1 == size2, "Tuples passed to apply_2 must have equal sizes");
    using Indices = std::make_index_sequence<size1>;
    apply_2_impl(std::forward<Function>(function), std::forward<Tuple1>(tuple1),
                 std::forward<Tuple2>(tuple2), Indices{});
}

#endif  // ELEMENTWISE_APPLY_HEADER_GUARD_HPP_
