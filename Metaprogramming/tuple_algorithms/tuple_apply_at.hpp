#if !defined(TUPLE_APPLY_AT_HEADER_GUARD_H_)
#define TUPLE_APPLY_AT_HEADER_GUARD_H_

#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

// The helper is necessary to allow for `Action` callables that can be called only with the
// element at the given index.
template <typename Tuple, typename Action, std::size_t I,
          bool = std::is_invocable_v<std::decay_t<Action>,
                                     decltype(std::get<I>(std::declval<Tuple&&>()))>>
struct tuple_apply_at_helper {
    constexpr void operator()(Tuple&&, Action&&) const noexcept
    {
        std::cerr << "\n" << __PRETTY_FUNCTION__ << "\n";
        // std::forward<Action>(action)(std::get<I>(std::forward<Tuple>(tuple)));
    }
};
template <typename Tuple, typename Action, std::size_t I>
struct tuple_apply_at_helper<Tuple, Action, I, true> {
    constexpr void operator()(Tuple&& tuple, Action&& action) const
    // noexcept(
    //     std::is_nothrow_invocable_v<Action, decltype(std::get<I>(std::declval<Tuple&&>()))>)
    {
        std::cerr << "\n" << __PRETTY_FUNCTION__ << "\n";
        std::forward<Action>(action)(std::get<I>(std::forward<Tuple>(tuple)));
    }
};

template <typename Tuple, typename Action, std::size_t... Is>
constexpr void tuple_apply_at_impl(Tuple&& tuple, Action&& action, std::size_t index,
                                   std::index_sequence<Is...>);

template <typename Tuple, typename Action>
constexpr void tuple_apply_at_impl(Tuple&&, Action&&, std::size_t, std::index_sequence<>)
{
}

template <typename Tuple, typename Action, std::size_t I, std::size_t... Is>
constexpr void tuple_apply_at_impl(Tuple&& tuple, Action&& action, std::size_t index,
                                   std::index_sequence<I, Is...>)
{
    if (index == I) {
        tuple_apply_at_helper<Tuple, Action, I>{}(std::forward<Tuple>(tuple),
                                                  std::forward<Action>(action));
    }
    else {
        tuple_apply_at_impl(std::forward<Tuple>(tuple), std::forward<Action>(action), index,
                            std::index_sequence<Is...>{});
    }
}

template <typename Tuple, typename Action>
constexpr void tuple_apply_at(Tuple&& tuple, Action&& action, std::size_t index)
{
    using Indices = std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>;
    tuple_apply_at_impl(std::forward<Tuple>(tuple), std::forward<Action>(action), index, Indices{});
}

// Adding a reliable noexcept declaration is difficult. An optymistic approach would be to check
// if the Action can be nothrow-invoked with any of the tuple elements. With C++17 this could be
// done as follows:
// noexcept((... || std::is_nothrow_invocable_v<Action,
//                                              decltype(std::get<Is>(std::declval<Tuple&&>()))>));
// And probably could also be achieved pre-C++17 with a tone more code. However this seems like
// wishfull thinking and is probably best avoided.

namespace alt {

template <typename Indices>
struct tuple_apply_at_impl;

template <>
struct tuple_apply_at_impl<std::index_sequence<>> {
    template <typename Tuple, typename Action>
    constexpr void operator()(Tuple&&, Action&&, std::size_t) const noexcept
    {
    }
};

template <std::size_t I, std::size_t... Is>
struct tuple_apply_at_impl<std::index_sequence<I, Is...>> {
    template <typename Tuple, typename Action>
    constexpr void operator()(Tuple&& tuple, Action&& action, std::size_t index) const noexcept
    {
        if (I == index) {
            tuple_apply_at_helper<Tuple, Action, I>{}(std::forward<Tuple>(tuple),
                                                      std::forward<Action>(action));
        }
        else {
            tuple_apply_at_impl<std::index_sequence<Is...>>{}(std::forward<Tuple>(tuple),
                                                              std::forward<Action>(action), index);
        }
    }
};

template <typename Tuple, typename Action>
constexpr void tuple_apply_at(Tuple&& tuple, Action&& action, std::size_t index) noexcept
{
    using Indices = std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>;
    tuple_apply_at_impl<Indices>{}(std::forward<Tuple>(tuple), std::forward<Action>(action), index);
}

}  // namespace alt

namespace bad {

// template <typename Tuple, typename Action,
//           std::size_t Size = std::tuple_size_v<std::decay_t<Tuple>>>
// struct tuple_apply_at_impl {
//     using self = tuple_apply_at_impl<Tuple, Action, Size>;
//     std::size_t index;

//     constexpr tuple_apply_at_impl(std::size_t i) noexcept : index{i}
//     {
//         // check index and throw in the ctor instead?
//         // save one compare operation and throw from the terminating apply specialization for now
//     }

//     template <std::size_t I = 0>
//     constexpr void apply(Tuple tuple, Action action) const noexcept
//     {
//         std::cerr << __PRETTY_FUNCTION__ << "\n";
//         if (I == index) {
//             std::forward<Action>(action)(std::get<I>(std::forward<Tuple>(tuple)));
//         }
//         else {
//             self::apply<I + 1>(std::forward<Tuple>(tuple), std::forward<Action>(action));
//         }
//     }

//     // Full specialization of an inner-template is invalid for a non-specialized outer template
//     template <>
//     constexpr void apply<Size>(Tuple tuple, Action action) const noexcept
//     {
//     }
// };

// template <typename Tuple, typename Action>
// decltype(auto) tuple_apply_at(Tuple&& tuple, Action&& action, std::size_t index) noexcept
// {
//     // If we had C++20 contracts this could be noexcept
//     // expects(index < std::tuple_size_v<std::decay_t<Tuple>>);
//     // constexpr auto size = std::tuple_size_v<std::decay_t<Tuple>>;
//     return tuple_apply_impl<Tuple, Action>{index}.apply(std::forward<Tuple>(tuple),
//                                                         std::forward<Action>(action));
// }

}  // namespace bad

#endif  // TUPLE_APPLY_AT_HEADER_GUARD_H_
