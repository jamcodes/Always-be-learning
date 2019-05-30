#if !defined(TUPLE_FOR_EACH_HEADER_GUARD_)
#define TUPLE_FOR_EACH_HEADER_GUARD_

#include <tuple>
#include <type_traits>
#include <utility>

template <typename Tuple, typename Function, std::size_t... Is>
constexpr void
tuple_for_each_impl(Tuple&& tuple, Function&& function, std::index_sequence<Is...>) noexcept(
    (... && std::is_nothrow_invocable_v<Function, std::tuple_element_t<Is, std::decay_t<Tuple>>>))
{
    (..., std::forward<Function>(function)(std::get<Is>(std::forward<Tuple>(tuple))));
}

template <typename Tuple, typename Function>
constexpr void tuple_for_each(Tuple&& tuple, Function&& function)
// noexcept(std::is_nothrow_invocable_v<tuple_for_each_impl<Tuple, Function)
{
    using Indices = std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>;
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

template <typename Tuple, typename Function, std::size_t... Is>
constexpr void tuple_for_each_impl(Tuple&&, Function&&, std::index_sequence<Is...>);

template <typename Tuple, typename Function>
constexpr void tuple_for_each_impl(Tuple&&, Function&&, std::index_sequence<>)
{
}

template <typename Tuple, typename Function, std::size_t I, std::size_t... Is>
constexpr void tuple_for_each_impl(Tuple&& tuple, Function&& function,
                                   std::index_sequence<I, Is...>)
{
    std::forward<Function>(function)(std::get<I>(std::forward<Tuple>(tuple)));
    tuple_for_each_impl(std::forward<Tuple>(tuple), std::forward<Function>(function),
                        std::index_sequence<Is...>{});
}

template <typename Tuple, typename Function>
constexpr void tuple_for_each(Tuple&& tuple, Function&& function)
{
    using Indices = std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>;
    return tuple_for_each_impl(std::forward<Tuple>(tuple), std::forward<Function>(function),
                               Indices{});
}

}  // namespace alt2

namespace alt3 {

template <typename Indices>
struct tuple_for_each_impl;

template <>
struct tuple_for_each_impl<std::index_sequence<>> {
    template <typename Tuple, typename Predicate>
    constexpr void operator()(Tuple&&, Predicate&&) const
    {
    }
};

template <std::size_t I, std::size_t... Is>
struct tuple_for_each_impl<std::index_sequence<I, Is...>> {
    template <typename Tuple, typename Function>
    constexpr void operator()(Tuple&& tuple, Function&& function) const
    {
        std::forward<Function>(function)(std::get<I>(std::forward<Tuple>(tuple)));
        tuple_for_each_impl<std::index_sequence<Is...>>{}(std::forward<Tuple>(tuple),
                                                          std::forward<Function>(function));
    }
};

template <typename Tuple, typename Function>
constexpr void tuple_for_each(Tuple&& tuple, Function&& function)
{
    using Indices = std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>;
    return tuple_for_each_impl<Indices>{}(std::forward<Tuple>(tuple),
                                          std::forward<Function>(function));
}

}  // namespace alt3

namespace alt4 {

template <std::size_t I>
struct tuple_for_each_apply_one {
    template <typename Tuple, typename Function>
    constexpr void operator()(Tuple&& tuple, Function&& function) const
    {
        std::forward<Function>(function)(std::get<I>(std::forward<Tuple>(tuple)));
    }
};

template <typename Indices>
struct tuple_for_each_impl;

template <>
struct tuple_for_each_impl<std::index_sequence<>> {
    template <typename Tuple, typename Function>
    constexpr void operator()(Tuple&&, Function&&) const
    {
    }
};

template <std::size_t I, std::size_t... Is>
struct tuple_for_each_impl<std::index_sequence<I, Is...>>
    : tuple_for_each_apply_one<I>, tuple_for_each_impl<std::index_sequence<Is...>> {
    using current = tuple_for_each_apply_one<I>;
    using remaining = tuple_for_each_impl<std::index_sequence<Is...>>;
    template <typename Tuple, typename Function>
    constexpr void operator()(Tuple&& tuple, Function&& function) const
    {
        current::operator()(std::forward<Tuple>(tuple), std::forward<Function>(function));
        remaining::operator()(std::forward<Tuple>(tuple), std::forward<Function>(function));
    }
};

template <typename Tuple, typename Function>
constexpr void tuple_for_each(Tuple&& tuple, Function&& function)
{
    using Indices = std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>;
    return tuple_for_each_impl<Indices>{}(std::forward<Tuple>(tuple),
                                          std::forward<Function>(function));
}

}  // namespace alt4

namespace bad {

// Compiles on clang, kills gcc - infinite template instantiation recursion

// template <typename Tuple, typename Function, std::size_t N>
// struct tuple_for_each_helper {
//     using self = alt2::tuple_for_each_helper<Tuple, Function, N>;

//     template <std::size_t I>
//     static constexpr void call(Tuple&& tuple, Function&& function)
//     {
//         std::forward<Function>(function)(std::get<I>(std::forward<Tuple>(tuple)));
//         self::call<I + 1>(std::forward<Tuple>(tuple), std::forward<Function>(function));
//     }

//     // Should not work - full specialization of an inner template
//     // of a non-specialized outer-template
//     template <>
//     static constexpr void call<N>(Tuple&&, Function&&)
//     {
//     }
// };

// template <typename Tuple, typename Function>
// constexpr void tuple_for_each(Tuple&& tuple, Function&& function)
// {
//     constexpr auto size = std::tuple_size_v<std::decay_t<Tuple>>;
//     alt2::tuple_for_each_helper<Tuple, Function, size>::template
//     call<0>(std::forward<Tuple>(tuple),
//                                                              std::forward<Function>(function));
// }

}  // namespace bad

#endif  // TUPLE_FOR_EACH_HEADER_GUARD_
