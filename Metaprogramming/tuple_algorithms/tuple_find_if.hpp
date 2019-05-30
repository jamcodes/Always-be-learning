#if !defined(TUPLE_FIND_IF_HEADER_GUARD_H_)
#define TUPLE_FIND_IF_HEADER_GUARD_H_

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

template <typename Indices>
struct tuple_find_if_impl;

template <std::size_t I, std::size_t... Is>
struct tuple_find_if_impl<std::index_sequence<I, Is...>> {
    template <typename Tuple, typename Predicate>
    constexpr auto operator()(Tuple&& tuple, Predicate&& predicate) const noexcept
    {
        if (std::forward<Predicate>(predicate)(std::get<I>(std::forward<Tuple>(tuple)))) {
            return I;
        }
        return tuple_find_if_impl<std::index_sequence<Is...>>{}(std::forward<Tuple>(tuple),
                                                                std::forward<Predicate>(predicate));
    }
};

template <std::size_t I>
struct tuple_find_if_impl<std::index_sequence<I>> {
    template <typename Tuple, typename Predicate>
    constexpr auto operator()(Tuple&& tuple, Predicate&& predicate) const noexcept
    {
        if (std::forward<Predicate>(predicate)(std::get<I>(std::forward<Tuple>(tuple)))) {
            return I;
        }
        return I + 1;
    }
};

template <typename Tuple, typename Predicate>
constexpr auto tuple_find_if(Tuple&& tuple, Predicate&& predicate)
{
    using Indices = std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>;
    return tuple_find_if_impl<Indices>{}(std::forward<Tuple>(tuple),
                                         std::forward<Predicate>(predicate));
}

namespace alt {

template <typename Tuple, typename Predicate, std::size_t... Is>
constexpr auto tuple_find_if_impl(Tuple&& tuple, Predicate&& predicate,
                                  std::index_sequence<Is...>) noexcept;

template <typename Tuple, typename Predicate>
constexpr auto tuple_find_if_impl(Tuple&&, Predicate&&, std::index_sequence<>) noexcept
{
    return std::tuple_size_v<std::decay_t<Tuple>>;
}

template <typename Tuple, typename Predicate, std::size_t I, std::size_t... Is>
constexpr auto tuple_find_if_impl(Tuple&& tuple, Predicate&& predicate,
                                  std::index_sequence<I, Is...>) noexcept
{
    if (std::forward<Predicate>(predicate)(std::get<I>(std::forward<Tuple>(tuple)))) {
        return I;
    }
    return tuple_find_if_impl(std::forward<Tuple>(tuple), std::forward<Predicate>(predicate),
                              std::index_sequence<Is...>{});
}

template <typename Tuple, typename Predicate>
constexpr auto tuple_find_if(Tuple&& tuple, Predicate&& predicate)
{
    using Indices = std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>;
    return alt::tuple_find_if_impl(std::forward<Tuple>(tuple), std::forward<Predicate>(predicate),
                                   Indices{});
}

}  // namespace alt

// compiles on clang
namespace bad {
// template <typename Tuple, typename Predicate, std::size_t Size>
// struct tuple_find_if_impl {
//     using self = tuple_find_if_impl<Tuple, Predicate, Size>;
//     template <std::size_t I = 0>
//     constexpr std::size_t call(Tuple tuple, Predicate predicate) noexcept
//     {
//         if (std::forward<Predicate>(predicate)(std::get<I>(std::forward<Tuple>(tuple)))) {
//             return I;
//         }
//         return self::call<I + 1>(std::forward<Tuple>(tuple), std::forward<Predicate>(predicate));
//     }
//     // This should not work - can not fully-specialize an inner-template of a
//     // non-specialized outer-template
//     template <>
//     constexpr std::size_t call<Size>(Tuple, Predicate)
//     {
//         return Size;
//     }
// };

// template <typename Tuple, typename Predicate>
// auto tuple_find_if(Tuple&& tuple, Predicate&& predicate)
// {
//     constexpr auto size = std::tuple_size_v<std::decay_t<Tuple>>;
//     return alt::tuple_find_if_impl<Tuple, Predicate, size>{}.call(
//         std::forward<Tuple>(tuple), std::forward<Predicate>(predicate));
// }
}  // namespace bad

#endif  // TUPLE_FIND_IF_HEADER_GUARD_H_
