#pragma once

#include <utility>
#include <type_traits>

template<typename InputIt>
using RequiresInputIterator = std::enable_if_t<
        std::is_convertible_v<typename std::iterator_traits<InputIt>::iterator_category, std::input_iterator_tag>>;

template<typename PropagateOnSwap> struct SwapAllocators;
template<> struct SwapAllocators<std::true_type> {
    template<typename Alloc>
    void operator()(Alloc& lhs, Alloc& rhs) const noexcept
    {
        using std::swap;
        swap(lhs, rhs);
    }
};
template<> struct SwapAllocators<std::false_type> {
    template<typename Alloc>
    void operator()(Alloc&, Alloc&) const noexcept {}
};
