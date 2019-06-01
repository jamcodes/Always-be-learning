#if !defined(OUTPUT_TRANSFORM_ITERATOR_HEADER_GUARD_HPP_)
#define OUTPUT_TRANSFORM_ITERATOR_HEADER_GUARD_HPP_

#include <iterator>
#include <type_traits>
#include <utility>

template <typename Iterator, typename Function>
class output_transform_iterator {
public:
    using iterator_category = std::output_iterator_tag;
    using value_type        = typename std::iterator_traits<Iterator>::value_type;
    using difference_type   = typename std::iterator_traits<Iterator>::difference_type;
    using pointer           = typename std::iterator_traits<Iterator>::pointer;
    using reference         = typename std::iterator_traits<Iterator>::reference;

    constexpr explicit output_transform_iterator(Iterator iterator, Function function) noexcept(
        std::is_nothrow_move_assignable_v<Iterator>&& std::is_nothrow_move_assignable_v<Function>)
        : iterator_{std::move(iterator)}, function_{std::move(function)}
    {
    }

    constexpr output_transform_iterator& operator++() noexcept(noexcept(std::declval<Iterator>()++))
    {
        ++iterator_;
        return *this;
    }

    constexpr output_transform_iterator operator++(int) noexcept(noexcept(std::declval<Iterator>()++) &&
                                                       noexcept(++std::declval<Iterator>()))
    {
        // auto res{*this};     // necessary?
        // ++*this;             // necessary?
        // return res;
        iterator_++;
        return *this;
    }

    constexpr output_transform_iterator& operator*() noexcept { return *this; }
    constexpr output_transform_iterator operator*() const noexcept { return *this; }

    template <typename T>
    constexpr output_transform_iterator& operator=(T&& value) noexcept
    {
        *iterator_ = function_(std::forward<T>(value));
        return *this;
    }

private:
    Iterator iterator_;
    Function function_;
};

template <typename Function>
class output_transformer {
public:
    constexpr explicit output_transformer(Function function) noexcept(
        std::is_nothrow_move_constructible_v<Function>)
        : function_{std::move(function)}
    {
    }

    template <typename Iterator>
    constexpr output_transform_iterator<Iterator, Function> operator()(Iterator iterator)
        const& noexcept(std::is_nothrow_constructible_v<
                        output_transform_iterator<Iterator, Function>, Iterator, Function>)
    {
        return output_transform_iterator<Iterator, Function>(std::move(iterator), function_);
    }

    template <typename Iterator>
        constexpr output_transform_iterator<Iterator, Function> operator()(Iterator iterator) &&
        noexcept(std::is_nothrow_constructible_v<output_transform_iterator<Iterator, Function>,
                                                 Iterator, Function>)
    {
        return output_transform_iterator<Iterator, Function>(std::move(iterator),
                                                             std::move(function_));
    }

private:
    Function function_{};
};

template <typename Function>
constexpr output_transformer<Function> make_output_transformer(Function function) noexcept(
    std::is_nothrow_constructible_v<output_transformer<Function>, Function>)
{
    return output_transformer<Function>{std::move(function)};
}

#endif  // OUTPUT_TRANSFORM_ITERATOR_HEADER_GUARD_HPP_
