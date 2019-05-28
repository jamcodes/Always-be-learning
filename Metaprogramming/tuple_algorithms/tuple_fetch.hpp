#if !defined(TUPLE_FETCH_HEADER_GUARD_H_)
#define TUPLE_FETCH_HEADER_GUARD_H_

#include <stdexcept>
#include <tuple>
#include <utility>

// This doesn't work - decltype(auto) deduces conflicting results, so it's likely impossible
// to return a refrence to a tuple's element using a runtime-index...

template <typename Tuple, std::size_t Size>
struct tuple_fetch_impl {
    using self = tuple_fetch_impl<Tuple, Size>;
    std::size_t index;
    constexpr tuple_fetch_impl(std::size_t i)  // noexcept
        : index{i}
    {
        // check index and throw in the ctor instead?
        // save one compare operation and throw from the terminating fetch specialization for now
    }

    template <std::size_t I = 0>
    constexpr decltype(auto) fetch(Tuple tuple) const
    {
        std::cerr << __PRETTY_FUNCTION__ << "\n";
        if (I == index) {
            return std::get<I>(std::forward<Tuple>(tuple));
        }
        return self::fetch<I + 1>(std::forward<Tuple>(tuple));
    }
    template <>
    constexpr decltype(auto) fetch<Size>(Tuple tuple) const
    {
        throw std::out_of_range{"Tuple index out of range"};
        return std::get<0>(std::forward<Tuple>(tuple));
    }
};

template <typename Tuple>
decltype(auto) tuple_fetch(Tuple&& tuple, std::size_t index)  // noexcept
{   
    // If we had C++20 contracts this could be noexcept
    // expects(index < std::tuple_size_v<std::decay_t<Tuple>>);
    constexpr auto size = std::tuple_size_v<std::decay_t<Tuple>>;
    return tuple_fetch_impl<Tuple, size>{index}.fetch(std::forward<Tuple>(tuple));
}

#endif  // TUPLE_FETCH_HEADER_GUARD_H_
