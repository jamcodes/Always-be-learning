#if !defined(TUPLE_APPLY_UNIQUE_HEADER_GUARD_HPP)
#define TUPLE_APPLY_UNIQUE_HEADER_GUARD_HPP

#include <tuple>
#include <type_traits>
#include <utility>

/**
 * tuple_apply_unique - invokes a callable `Action` with argument from a given `Tuple` found at
 * index `I` if the argument meets the given `Predicate`. Stops iterating over the elements of the
 * tuple after finding the first argument which meets the predicate.
 */

template <typename Action, typename T, bool = std::is_invocable_v<std::decay_t<Action>, T&&>>
struct tuple_apply_unique_maybe_invoke {
    constexpr void operator()(Action&&, T&&) noexcept
    { /* nop */
    }
};

template <typename Action, typename T>
struct tuple_apply_unique_maybe_invoke<Action, T, true> {
    constexpr void operator()(Action&& action, T&& value) const noexcept
    {
        std::forward<Action>(action)(std::forward<T>(value));
    }
};

template <typename Tuple, typename Action, typename Predicate, std::size_t... Is>
constexpr void tuple_apply_unique_impl(Tuple&& tuple, Action&& action, Predicate&& predicate,
                                       std::index_sequence<Is...>) noexcept;

template <typename Tuple, typename Action, typename Predicate, std::size_t I>
constexpr void tuple_apply_unique_impl(Tuple&& tuple, Action&& action, Predicate&& predicate,
                                       std::index_sequence<I>) noexcept
{
    auto&& value = std::get<I>(std::forward<Tuple>(tuple));
    if (std::forward<Predicate>(predicate)(value)) {
        tuple_apply_unique_maybe_invoke<Action, decltype(value)>{}(
            std::forward<Action>(action), std::forward<decltype(value)>(value));
    }
}

template <typename Tuple, typename Action, typename Predicate, std::size_t I, std::size_t... Is>
constexpr void tuple_apply_unique_impl(Tuple&& tuple, Action&& action, Predicate&& predicate,
                                       std::index_sequence<I, Is...>) noexcept
{
    auto&& value = std::get<I>(std::forward<Tuple>(tuple));
    // TODO: wrap the predicate to allow it to be "invoked" with an invalid type (one it can not be
    // invoked with) - return false if the types are not compatible. This would allow using
    // tuple_apply_unique with truly heterogenous tuples and incomplete predicates - ones that don't
    // provide overloads for all types held by the tuple.
    if (std::forward<Predicate>(predicate)(value)) {
        tuple_apply_unique_maybe_invoke<Action, decltype(value)>{}(
            std::forward<Action>(action), std::forward<decltype(value)>(value));
        return;
    }
    tuple_apply_unique_impl(std::forward<Tuple>(tuple), std::forward<Action>(action),
                            std::forward<Predicate>(predicate), std::index_sequence<Is...>{});
}

template <typename Tuple, typename Action, typename Predicate>
constexpr void tuple_apply_unique(Tuple&& tuple, Action&& action, Predicate&& predicate) noexcept
{
    using Indices = std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>;
    tuple_apply_unique_impl(std::forward<Tuple>(tuple), std::forward<Action>(action),
                       std::forward<Predicate>(predicate), Indices{});
}

namespace alt {

template <typename Indices>
struct tuple_apply_unique_impl;

template <>
struct tuple_apply_unique_impl<std::index_sequence<>> {
    template <typename Tuple, typename Action, typename Predicate>
    constexpr void operator()(Tuple&&, Action&&, Predicate&&) const noexcept
    {
    }
};

template <std::size_t I, std::size_t... Is>
struct tuple_apply_unique_impl<std::index_sequence<I, Is...>> {
    template <typename Tuple, typename Action, typename Predicate>
    constexpr void operator()(Tuple&& tuple, Action&& action, Predicate&& predicate) const noexcept
    {
        if (std::forward<Predicate>(predicate)(std::get<I>(std::forward<Tuple>(tuple)))) {
            tuple_apply_unique_maybe_invoke<Action,
                                            decltype(std::get<I>(std::forward<Tuple>(tuple)))>{}(
                std::forward<Action>(action), std::get<I>(std::forward<Tuple>(tuple)));
            return;
        }
        tuple_apply_unique_impl<std::index_sequence<Is...>>{}(std::forward<Tuple>(tuple),
                                                              std::forward<Action>(action),
                                                              std::forward<Predicate>(predicate));
    }
};

template <typename Tuple, typename Action, typename Predicate>
constexpr void tuple_apply_unique(Tuple&& tuple, Action&& action, Predicate&& predicate) noexcept
{
    using Indices = std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>;
    tuple_apply_unique_impl<Indices>{}(std::forward<Tuple>(tuple), std::forward<Action>(action),
                                  std::forward<Predicate>(predicate));
}

}  // namespace alt

#endif  // TUPLE_APPLY_UNIQUE_HEADER_GUARD_HPP
