#if !defined(OUTPUT_FILTER_ITERATOR_HEADER_GUARD_HPP_)
#define OUTPUT_FILTER_ITERATOR_HEADER_GUARD_HPP_

#include <iterator>
#include <type_traits>
#include <utility>

template <typename Iterator, typename Predicate>
class output_filter_iterator {
    using self = output_filter_iterator;

public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;

    constexpr explicit output_filter_iterator(Iterator iterator, Predicate predicate) noexcept(
        std::is_nothrow_move_constructible_v<Iterator>&&
            std::is_nothrow_move_constructible_v<Iterator>)
        : iterator_{std::move(iterator)}, predicate_{std::move(predicate)}
    {
    }

    constexpr self& operator++() noexcept
    {
        // We want to increment the iterator only if the predicate yields true,
        // but we also want to avoid calling the predicate twice - so don't increment 
        // the iterator here - do it in the assignment operator
        // STL algorithms are not allowed to increment an output iterator twice, before assigning
        // to it, so we won't break anything by moving the increment to the output operator
        // ++iterator_;
        return *this;
    }
    constexpr self& operator++(int) noexcept
    {
        // See above for reasoning
        // auto res{*this};
        // iterator_++;
        // return res;
        return *this;
    }
    constexpr self& operator*() noexcept { return *this; }

    template <typename T>
    constexpr self& operator=(T&& value) noexcept
    {
        if (predicate_(value)) {
            *iterator_ = std::forward<T>(value);
            ++iterator_;
        }
        return *this;
    }

private:
    Iterator iterator_;
    Predicate predicate_;
};

template <typename Predicate>
class output_filter {
public:
    explicit constexpr output_filter(Predicate predicate) noexcept(
        std::is_nothrow_move_constructible_v<Predicate>)
        : predicate_{std::move(predicate)}
    {
    }

    template <typename Iterator>
        constexpr auto operator()(Iterator iterator) const&
        noexcept(std::is_nothrow_constructible_v<output_filter_iterator<Iterator, Predicate>,
                                                 Iterator, Predicate>)
    {
        return output_filter_iterator<Iterator, Predicate>(std::move(iterator), predicate_);
    }

    template <typename Iterator>
        constexpr auto operator()(Iterator iterator) &&
        noexcept(std::is_nothrow_constructible_v<output_filter_iterator<Iterator, Predicate>,
                                                 Iterator, Predicate>)
    {
        return output_filter_iterator<Iterator, Predicate>(std::move(iterator),
                                                           std::move(predicate_));
    }

private:
    Predicate predicate_;
};

template <typename Predicate>
constexpr auto make_output_filter(Predicate predicate) noexcept(
    std::is_nothrow_move_constructible_v<Predicate>)
{
    return output_filter<Predicate>{std::move(predicate)};
}

#endif  // OUTPUT_FILTER_ITERATOR_HEADER_GUARD_HPP_
