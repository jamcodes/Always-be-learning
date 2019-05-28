#if !defined(TUPLE_FIND_IF_HEADER_GUARD_H_)
#define TUPLE_FIND_IF_HEADER_GUARD_H_

#include <cstddef>
#include <tuple>
#include <utility>

template <typename Tuple, typename Predicate, std::size_t Size>
struct tuple_find_if_impl {
    using self = tuple_find_if_impl<Tuple, Predicate, Size>;
    template <std::size_t I = 0>
    constexpr std::size_t call(Tuple tuple, Predicate predicate) noexcept
    {
        if (std::forward<Predicate>(predicate)(std::get<I>(std::forward<Tuple>(tuple)))) {
            return I;
        }
        return self::call<I + 1>(std::forward<Tuple>(tuple), std::forward<Predicate>(predicate));
    }

    template <>
    constexpr std::size_t call<Size>(Tuple, Predicate)
    {
        return Size;
    }
};

template <typename Tuple, typename Predicate>
auto tuple_find_if(Tuple&& tuple, Predicate&& predicate)
{
    constexpr auto size = std::tuple_size_v<std::decay_t<Tuple>>;
    return tuple_find_if_impl<Tuple, Predicate, size>{}.call(std::forward<Tuple>(tuple),
                                                             std::forward<Predicate>(predicate));
}

#endif  // TUPLE_FIND_IF_HEADER_GUARD_H_
