#if !defined(OUTPUT_DEMUX_ITERATOR_HEADER_GUARD_HPP_)
#define OUTPUT_DEMUX_ITERATOR_HEADER_GUARD_HPP_

#include <tuple>
#include <type_traits>
#include <utility>
#include "tuple_apply_unique.hpp"

template<typename... Branches>
class output_demux_iterator {
public:
    constexpr explicit output_demux_iterator(Branches... branches) noexcept
        : branches_{std::make_tuple(std::move(branches)...)}
    {
    }

    constexpr self& operator*() noexcept { return *this; }
    constexpr self& operator++() noexcept { return *this; }
    constexpr self& operator++(int) noexcept { return ++*this; }

    template<typename T>
    constexpr self& operator=(T&& value) noexcept
    {
        tuple_apply_unique(branches_, [&value](auto&& branch) noexcept{
            *branch.iterator = std::forward<T>(value); ++branch.iterator;
        },
        [&value](auto const& branch) noexcept { return branch.predicate(value); }
        );
        return *this;
    }
private:
    using self = output_demux_iterator;
    std::tuple<Branches...> branches_;
};

template<typename Predicate, typename Iterator>
struct demux_branch {
    Predicate predicate;
    Iterator iterator;
};

template<typename Predicate>
class demux_if {
public:
    constexpr explicit demux_if(Predicate predicate) noexcept
        : predicate_{std::move(predicate)} { }

    template<typename Iterator>
    constexpr auto send_to(Iterator iterator) noexcept {
        return output_demux_iterator<demux_branch<Predicate, Iterator>>{predicate_, std::move(iterator)};
    }
private:
    Predicate predicate_;
};

#endif // OUTPUT_DEMUX_ITERATOR_HEADER_GUARD_HPP_
