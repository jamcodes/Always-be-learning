#if !defined(TUPLE_ACCUMULATE_HEADER_GUARD_H_)
#define TUPLE_ACCUMULATE_HEADER_GUARD_H_

#include <tuple>
#include <type_traits>
#include <utility>

template <typename Indices>
struct tuple_accumulate_impl;

template <std::size_t I>
struct tuple_accumulate_impl<std::index_sequence<I>> {
    template <typename Tuple, typename T, typename Function>
    constexpr inline T operator()(Tuple&& tuple, T&& init, Function&& function) const
        noexcept(std::is_nothrow_invocable_v<Function, T&&,
                                             decltype(std::get<I>(std::forward<Tuple>(tuple)))>)
    {
        return std::forward<Function>(function)(std::forward<T>(init),
                                                std::get<I>(std::forward<Tuple>(tuple)));
    }
};

template <std::size_t I, std::size_t... Is>
struct tuple_accumulate_impl<std::index_sequence<I, Is...>> {
    template <typename Tuple, typename T, typename Function>
    constexpr inline T operator()(Tuple&& tuple, T&& init, Function&& function) const
        noexcept(std::is_nothrow_invocable_v<Function, T&&,
                                             decltype(std::get<I>(std::forward<Tuple>(tuple)))>&&
                     std::is_nothrow_invocable_v<tuple_accumulate_impl<std::index_sequence<Is...>>,
                                                 Tuple&&, T, Function&&>)
    {
        return tuple_accumulate_impl<std::index_sequence<Is...>>{}(
            std::forward<Tuple>(tuple),
            function(std::forward<T>(init), std::get<I>(std::forward<Tuple>(tuple))),
            std::forward<Function>(function));
    }
};

template <typename Tuple, typename T, typename Function>
T tuple_accumulate(Tuple&& tuple, T&& init, Function&& func)
{
    using Indices = std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>;
    return tuple_accumulate_impl<Indices>{}(std::forward<Tuple>(tuple), std::forward<T>(init),
                                            std::forward<Function>(func));
}



template <typename Tuple, typename T, typename Function, std::size_t... Is>
decltype(auto) tupleAccumulateImpl(
    Tuple&& tuple, T&& init, Function&& function,
    std::index_sequence<
        Is...>) noexcept((... &&
                          std::is_nothrow_invocable_v<
                              Function, decltype(std::get<Is>(std::forward<Tuple>(tuple))), T&&>))
{
    auto accumulator = [&init, f{std::forward<Function>(function)}](auto&& val) {
        f(init, std::forward<decltype(val)>(val));
    };
    (..., accumulator(std::get<Is>(std::forward<Tuple>(tuple))));
    return std::forward<T>(init);
}

template <typename Tuple, typename T, typename Function>
decltype(auto) tupleAccumulate(Tuple&& tuple, T&& init, Function&& function) noexcept(
    noexcept(tupleAccumulateImpl(std::forward<Tuple>(tuple), std::forward<T>(init), std::forward<Function>(function),
                                 std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>{})))
{
    using Indices = std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>;
    return tupleAccumulateImpl(std::forward<Tuple>(tuple), std::forward<T>(init),
                                        std::forward<Function>(function), Indices{});
}

#endif
