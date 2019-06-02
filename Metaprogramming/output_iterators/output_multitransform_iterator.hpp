#if !defined(OUTPUT_MULTITRANSFORM_ITERATOR_HEADER_GUARD_HPP_)
#define OUTPUT_MULTITRANSFORM_ITERATOR_HEADER_GUARD_HPP_

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

template <typename Iterators, typename Functions>
class output_multitransform_iterator;

template <typename... Iterators, typename... Functions>
class output_multitransform_iterator<std::tuple<Iterators...>, std::tuple<Functions...>> {
    using self = output_multitransform_iterator;

public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;

    template <typename FuncTuple>
    explicit constexpr output_multitransform_iterator(FuncTuple&& functions,
                                                      Iterators... iterators) noexcept
        : iterators_{std::make_tuple(std::move(iterators)...)},
          functions_{std::forward<FuncTuple>(functions)}
    {
    }

    explicit constexpr output_multitransform_iterator(std::tuple<Iterators...>&& iterators,
                                                      std::tuple<Functions...>&& functions) noexcept
        : iterators_{std::move(iterators)}, functions_{std::move(functions)}
    {
    }

    constexpr self& operator*() noexcept { return *this; }
    constexpr self& operator++() noexcept
    {
        // apply operator++() to all iterators...
        tuple_for_each(iterators_, [](auto& iter) noexcept { ++iter; });
        return *this;
    }
    constexpr self& operator++(int) noexcept
    {
        // same
        return ++*this;
    }

    template <typename T>
    constexpr self& operator=(T&& value) noexcept
    {
        // apply a function to the value and corresponding iterators
        elementwise_apply(
            [&value](auto&& iterator, auto&& function) noexcept {
                *std::forward<decltype(iterator)>(iterator) =
                    std::forward<decltype(function)>(function)(std::forward<T>(value));
            },
            iterators_, functions_);
        return *this;
    }

private:
    std::tuple<Iterators...> iterators_;
    std::tuple<Functions...> functions_;
};

template <typename... Functions>
class multitransform {
public:
    explicit constexpr multitransform(Functions... functions) noexcept
        : functions_{std::move(functions)...}
    {
    }

    template <typename... Iterators>
    constexpr auto operator()(Iterators... iterators) const& noexcept
    {
        return output_multitransform_iterator<std::tuple<Iterators...>, std::tuple<Functions...>>{
            functions_, std::move(iterators)...};
    }

    template <typename... Iterators>
    constexpr auto operator()(Iterators... iterators) && noexcept
    {
        return output_multitransform_iterator<std::tuple<Iterators...>, std::tuple<Functions...>>{
            std::move(functions_), std::move(iterators)...};
    }

private:
    std::tuple<Functions...> functions_;
};

template <typename... Functions>
multitransform(Functions...)->multitransform<Functions...>;

template <typename... Functions>
constexpr auto make_multitransform(Functions&&... functions) noexcept
{
    return multitransform<std::decay_t<Functions>...>(std::forward<Functions>(functions)...);
}

#endif  // OUTPUT_MULTITRANSFORM_ITERATOR_HEADER_GUARD_HPP_
