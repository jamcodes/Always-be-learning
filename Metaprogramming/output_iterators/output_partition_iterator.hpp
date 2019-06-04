#if !defined(OUTPUT_PARTITION_ITERATOR_HEADER_GUARD_HPP_)
#define OUTPUT_PARTITION_ITERATOR_HEADER_GUARD_HPP_

#include <iterator>
#include <type_traits>
#include <utility>

template <typename Predicate, typename TrueIterator, typename FalseIterator>
class output_partition_iterator {
    using self = output_partition_iterator;

public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using reference = void;
    using pointer = void;

    explicit constexpr output_partition_iterator(Predicate predicate, TrueIterator true_iterator,
                                                 FalseIterator false_iterator) noexcept
        : predicate_{std::move(predicate)},
          true_iterator_{std::move(true_iterator)},
          false_iterator_{std::move(false_iterator)}
    {
    }

    constexpr self& operator*() noexcept { return *this; }
    constexpr self& operator++() noexcept { return *this; }
    constexpr self& operator++(int) noexcept { return ++*this; }

    template <typename T>
    constexpr self& operator=(T&& value) noexcept
    {
        if (predicate_(value)) {
            *true_iterator_ = std::forward<T>(value);
            ++true_iterator_;
        }
        else {
            *false_iterator_ = std::forward<T>(value);
            ++false_iterator_;
        }
        return *this;
    }

private:
    Predicate predicate_;
    TrueIterator true_iterator_;
    FalseIterator false_iterator_;
};

template <typename Predicate>
class output_partitioner {
public:
    explicit constexpr output_partitioner(Predicate predicate) noexcept
        : predicate_{std::move(predicate)}
    {
    }

    template <typename TrueIterator, typename FalseIterator>
    constexpr auto operator()(TrueIterator true_iterator,
                              FalseIterator false_iterator) const& noexcept
    {
        return output_partition_iterator<Predicate, TrueIterator, FalseIterator>(
            predicate_, std::move(true_iterator), std::move(false_iterator));
    }

    template <typename TrueIterator, typename FalseIterator>
    constexpr auto operator()(TrueIterator true_iterator,
                              FalseIterator false_iterator) && noexcept
    {
        return output_partition_iterator<Predicate, TrueIterator, FalseIterator>(
            std::move(predicate_), std::move(true_iterator), std::move(false_iterator));
    }

private:
    Predicate predicate_;
};

template<typename Predicate>
constexpr auto partition(Predicate&& predicate) noexcept
{
    return output_partitioner<std::decay_t<Predicate>>{std::forward<Predicate>(predicate)};
}

#endif  // OUTPUT_PARTITION_ITERATOR_HEADER_GUARD_HPP_
