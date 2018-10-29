#pragma once

#include <iostream>
#include <type_traits>
#include <utility>
#include <boost/type_index.hpp>

using boost::typeindex::type_id_with_cvr;


template<std::size_t Index, typename T>
class TupleElement
{
private:
    T value_{};
public:
    using type = T;

    constexpr TupleElement() = default;

    template<typename U /* ,typename = std::enable_if_t<std::is_convertible_v<U, T>> */>
    constexpr TupleElement(U&& value) noexcept(std::is_nothrow_constructible_v<T,U>)
        : value_{std::forward<U>(value)}
        {
            std::cerr << "TupleElement<" << Index << ", "
                << type_id_with_cvr<T>().pretty_name() << ">\n";
        }

    constexpr T&& Get() && noexcept { return std::move(value_); }
    constexpr T& Get() & noexcept { return value_; }
    constexpr T const& Get() const& noexcept { return value_; }
};

template<std::size_t I, typename T>
using TupleElement_t = typename TupleElement<I,T>::type;


template<std::size_t I, typename U>
inline constexpr TupleElement_t<I,U>& Get(TupleElement<I, U>& te) noexcept;

template<std::size_t I, typename U>
inline constexpr TupleElement_t<I,U> const& Get(TupleElement<I, U> const& te) noexcept;

template<std::size_t I, typename U>
inline constexpr TupleElement_t<I,U>&& Get(TupleElement<I,U>&& te) noexcept;


template<typename Indices, typename... Ts>
class Tuple_impl;


// inherit from element<0, T0>, element<1, T1> ...
template<std::size_t... Is, typename... Ts>
class Tuple_impl<std::index_sequence<Is...>, Ts...> : private TupleElement<Is, Ts>...
{
    template<typename...> friend class Tuple;
public:
    constexpr Tuple_impl() = default;
    template<typename... Types
        ,typename = std::enable_if_t<sizeof...(Types) == sizeof...(Ts)
        /* && (... && std::is_constructible_v<Ts,Types>) */>>
    constexpr Tuple_impl(Types&&... ts) noexcept
        : TupleElement<Is,Types>{std::forward<Types>(ts)}... { }


    template<std::size_t I, typename T, typename... Types>
    friend inline constexpr decltype(auto) GET(Tuple_impl<T, Types...>& t) noexcept;

    template<std::size_t I, typename T, typename... Types>
    friend inline constexpr decltype(auto) GET(Tuple_impl<T, Types...> const& t) noexcept;

    template<std::size_t I, typename T, typename... Types>
    friend inline constexpr decltype(auto) GET(Tuple_impl<T, Types...>&& t) noexcept;
};

// template<typename... Types>
// Tuple_impl(Types&&...) -> Tuple_impl<std::make_index_sequence<sizeof...(Types)>, Types...>;

template<typename... Ts>
class Tuple : public Tuple_impl<std::make_index_sequence<sizeof...(Ts)>, Ts...>
{
    using Base = Tuple_impl<std::make_index_sequence<sizeof...(Ts)>, Ts...>;
public:
    constexpr Tuple() = default;

    template<typename... Types>
    constexpr Tuple(Types&&... ts) noexcept
        : Base(std::forward<Types>(ts)...) { }
};

template<typename... Ts>
inline constexpr auto makeTuple(Ts&&... ts) noexcept
{
    return Tuple<Ts...>(std::forward<Ts>(ts)...);
}

template<std::size_t I, typename U>
inline constexpr TupleElement_t<I,U>& Get(TupleElement<I, U>& te) noexcept
{
        return te.Get();
}

template<std::size_t I, typename U>
inline constexpr TupleElement_t<I,U>const& Get(TupleElement<I, U> const& te) noexcept
{
        return te.Get();
}

template<std::size_t I, typename U>
inline constexpr TupleElement_t<I,U>&& Get(TupleElement<I, U>&& te) noexcept
{
        return te.Get();
}

// TODO: rename GET to `get` and rename `get` to `get_impl`
template<std::size_t I, typename T, typename... Types>
inline constexpr decltype(auto) GET(Tuple_impl<T, Types...>& t) noexcept
{
    return Get<I>(t);
}
template<std::size_t I, typename T, typename... Types>
inline constexpr decltype(auto) GET(Tuple_impl<T, Types...> const& t) noexcept
{
    return Get<I>(t);
}
template<std::size_t I, typename T, typename... Types>
inline constexpr decltype(auto) GET(Tuple_impl<T, Types...>&& t) noexcept
{
    return Get<I>(t);
}