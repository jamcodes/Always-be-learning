#if !defined(OUTPUT_DEMUX_ITERATOR_HEADER_GUARD_HPP_)
#define OUTPUT_DEMUX_ITERATOR_HEADER_GUARD_HPP_

#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>
#include "tuple_apply_unique.hpp"

/**
 * An `output_demux_iterator` takes a number of predicates, each associated to an iterator and
 * sends the given value to the first iterator for which the predicate is met.
 */

template <typename... Branches>
class output_demux_iterator {
    using self = output_demux_iterator;

public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using reference = void;
    using pointer = void;

    constexpr explicit output_demux_iterator(Branches... branches) noexcept
        : branches_{std::make_tuple(std::move(branches)...)}
    {
    }

    constexpr self& operator*() noexcept { return *this; }
    constexpr self& operator++() noexcept { return *this; }
    constexpr self& operator++(int) noexcept { return ++*this; }

    template <typename T>
    constexpr self& operator=(T&& value) noexcept
    {
        tuple_apply_unique(
            branches_,
            [&value](auto&& branch) noexcept {
                *branch.iterator = std::forward<T>(value);
                ++branch.iterator;
            },
            [&value](auto const& branch) noexcept { return branch.predicate(value); });
        return *this;
    }

private:
    std::tuple<Branches...> branches_;
};

template <typename Predicate, typename Iterator>
struct demux_branch {
    Predicate predicate;
    Iterator iterator;
};

template <typename Predicate>
class Demux_if {
public:
    constexpr explicit Demux_if(Predicate predicate) noexcept : predicate_{std::move(predicate)} {}

    template <typename Iterator>
    constexpr auto send_to(Iterator&& iterator) noexcept
    {
        return demux_branch<Predicate, std::decay_t<Iterator>>{predicate_,
                                                               std::forward<Iterator>(iterator)};
    }

private:
    Predicate predicate_;
};

template <typename Predicate>
constexpr auto demux_if(Predicate&& predicate) noexcept
{
    return Demux_if<std::decay_t<Predicate>>{std::forward<Predicate>(predicate)};
}

template <typename... Branches>
constexpr auto demux(Branches&&... branches) noexcept
{
    return output_demux_iterator<std::decay_t<Branches>...>{std::forward<Branches>(branches)...};
}

#endif  // OUTPUT_DEMUX_ITERATOR_HEADER_GUARD_HPP_
