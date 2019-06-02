#if !defined(UNZIP_ITERATOR_HEADER_GUARD_HPP_)
#define UNZIP_ITERATOR_HEADER_GUARD_HPP_

#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>
#include "elementwise_apply.hpp"

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

template <typename... Iterators>
class output_unzip_iterator {
public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;

    constexpr explicit output_unzip_iterator(Iterators... iterators) noexcept
        : iterators_{std::make_tuple(std::move(iterators)...)}
    {
    }

    constexpr output_unzip_iterator& operator*() noexcept { return *this; }
    constexpr output_unzip_iterator operator*() const noexcept { return *this; }
    constexpr output_unzip_iterator& operator++() noexcept
    {
        // apply operator++ to all iterators in the tuple
        tuple_for_each(iterators_, [](auto& iter) noexcept { ++iter; });
        return *this;
    }
    constexpr output_unzip_iterator& operator++(int) noexcept
    {
        // same
        return ++*this;
    }
    template <typename Tuple>
    constexpr output_unzip_iterator& operator=(Tuple&& tuple) noexcept
    {
        // unwrap the given Tuple and iterators_ -> assign elements of the Tuple to corresponding
        // iterators_ tuple indices
        elementwise_apply(
            [](auto&& iter, auto&& value) noexcept {
                *std::forward<decltype(iter)>(iter) = std::forward<decltype(value)>(value);
            },
            iterators_, std::forward<Tuple>(tuple));
        return *this;
    }

private:
    std::tuple<Iterators...> iterators_;
};

template <typename... Iterators>
constexpr auto unzip(Iterators... iterators) noexcept
{
    return output_unzip_iterator<Iterators...>(std::move(iterators)...);
}

namespace alt {
#include "output_multitransform_iterator.hpp"
// alternative implementation using multitransform output iterator
template <std::size_t... Is>
constexpr auto unzip_impl(std::index_sequence<Is...>) noexcept
{
    return make_multitransform([](auto&& tuple) noexcept {
        return std::get<Is>(std::forward<decltype(tuple)>(tuple));
    }...);
}
template <typename... Iterators>
constexpr auto unzip(Iterators... iterators) noexcept
{
    return unzip_impl(std::index_sequence_for<Iterators...>{})(std::move(iterators)...);
}
}  // namespace alt

#endif  // UNZIP_ITERATOR_HEADER_GUARD_HPP_
