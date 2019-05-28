#if !defined(TUPLE_APPLY_AT_HEADER_GUARD_H_)
#define TUPLE_APPLY_AT_HEADER_GUARD_H_

#include <stdexcept>
#include <tuple>
#include <utility>

template <typename Tuple, typename Action, std::size_t Size>
struct tuple_apply_at_impl {
    using self = tuple_apply_at_impl<Tuple, Size>;
    std::size_t index;
    constexpr tuple_apply_at_impl(std::size_t i)  // noexcept
        : index{i}
    {
        // check index and throw in the ctor instead?
        // save one compare operation and throw from the terminating fetch specialization for now
    }

    template <std::size_t I = 0>
    constexpr decltype(auto) apply(Tuple tuple, Action action) const
    {
        std::cerr << __PRETTY_FUNCTION__ << "\n";
        if (I == index) {
            return std::get<I>(std::forward<Tuple>(tuple));
        }
        return self::apply<I + 1>(std::forward<Tuple>(tuple));
    }
    template <>
    constexpr decltype(auto) apply<Size>(Tuple tuple) const
    {
        throw std::out_of_range{"Tuple index out of range"};
        return std::get<0>(std::forward<Tuple>(tuple));
    }
};

template <typename Tuple, typename Action>
decltype(auto) tuple_apply_at(Tuple&& tuple, Action&& action, std::size_t index)  // noexcept
{
    // If we had C++20 contracts this could be noexcept
    // expects(index < std::tuple_size_v<std::decay_t<Tuple>>);
    constexpr auto size = std::tuple_size_v<std::decay_t<Tuple>>;
    return tuple_fetch_impl<Tuple, Action, size>{index}.apply(std::forward<Tuple>(tuple),
                                                              std::forward<Action>(action));
}

#endif  // TUPLE_APPLY_AT_HEADER_GUARD_H_
