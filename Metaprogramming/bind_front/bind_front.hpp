#pragma once

#include <utility>
#include <type_traits>
#include <functional>
#include <tuple>

// C++20 implementation
// template<typename Function, typename... Args>
// constexpr auto bind_front_20(Function&& func, Args&&... args) noexcept
// {
//     return [f=std::forward<Function>(func)
//            ,...bound_args=std::forward<Args>(args) ](auto&&... rest) noexcept(
//                std::is_nothrow_invocable_v<Function, Args&&..., decltype(rest)...>)
//                -> decltype(auto)
//             {
//                 return std::invoke(bound_args..., std::forward<decltype(rest)>(rest)...);
//             }
// }

// C++17
template <typename From, typename To>
using enable_if_convertible = typename std::enable_if<
    !std::is_same<typename std::decay<From>::type, To>::value
    && !std::is_base_of<To, typename std::decay<From>::type>::value
    && std::is_convertible<typename std::decay<From>::type, To>::value>::type;

template<typename Function, typename... Args>
class BindFront {
public:
    template<typename... As>
    explicit BindFront(Function&& f, As&&... as)
        noexcept(std::is_nothrow_constructible_v<Function, Function&&> &&
                                std::is_nothrow_constructible_v<std::tuple<Args...>, As&&...>)
    : f_{std::move(f)}
    , bound_args_{std::forward<As>(as)...}
    {
    }

    template<typename... As>
    explicit BindFront(Function const& f, As&&... as)
        noexcept(std::is_nothrow_constructible_v<Function, Function&&> &&
                                std::is_nothrow_constructible_v<std::tuple<Args...>, As&&...>)
    : f_{f}
    , bound_args_{std::forward<As>(as)...}
    {
    }

    BindFront(BindFront const&) = default;
    BindFront(BindFront&&) noexcept = default;
    BindFront& operator=(BindFront const&) = default;
    BindFront& operator=(BindFront&&) noexcept = default;
    ~BindFront() = default;

    template<typename... As>
    inline constexpr decltype(auto) operator()(As&&... as) const
        noexcept(std::is_nothrow_invocable_v<Function, Args..., As...>)
    {
        using Indices = std::index_sequence_for<Args...>;
        return do_call(Indices{}, std::forward<As>(as)...);
    }

private:
    template<std::size_t... Is, typename... As>
    inline constexpr decltype(auto) do_call(std::index_sequence<Is...>, As&&... as) const noexcept(std::is_nothrow_invocable_v<Function, Args..., As...>)
    {
        return std::invoke(f_, std::get<Is>(bound_args_)..., std::forward<As>(as)...);
    }
    Function f_;
    std::tuple<Args...> bound_args_;
};

template<typename Function, typename... Args>
BindFront(Function f, Args...) -> BindFront<Function, Args...>;

template<typename Function, typename... Args>
constexpr auto bind_front(Function&& function, Args&&... args) noexcept
{
    return BindFront<std::decay_t<Function>, std::decay_t<Args>...>{std::forward<Function>(function),
                                        std::forward<Args>(args)...};
}
