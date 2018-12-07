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
inline constexpr TupleElement_t<I,U>& Get_impl(TupleElement<I, U>& te) noexcept;

template<std::size_t I, typename U>
inline constexpr TupleElement_t<I,U> const& Get_impl(TupleElement<I, U> const& te) noexcept;

template<std::size_t I, typename U>
inline constexpr TupleElement_t<I,U>&& Get_impl(TupleElement<I,U>&& te) noexcept;


template<typename Indices, typename... Ts>
class Tuple_impl;


// inherit from element<0, T0>, element<1, T1> ...
template<std::size_t... Is, typename... Ts>
class Tuple_impl<std::index_sequence<Is...>, Ts...> : private TupleElement<Is, Ts>...
{
public:
    constexpr Tuple_impl() = default;
    template<typename... Types
        ,typename = std::enable_if_t<sizeof...(Types) == sizeof...(Ts)
        && (... && std::is_constructible_v<Ts,Types>)>>
    constexpr Tuple_impl(Types&&... ts) noexcept
        : TupleElement<Is,Ts>{std::forward<Types>(ts)}... { }


    template<std::size_t I, typename T, typename... Types>
    friend inline constexpr decltype(auto) Get(Tuple_impl<T, Types...>& t) noexcept;

    template<std::size_t I, typename T, typename... Types>
    friend inline constexpr decltype(auto) Get(Tuple_impl<T, Types...> const& t) noexcept;

    template<std::size_t I, typename T, typename... Types>
    friend inline constexpr decltype(auto) Get(Tuple_impl<T, Types...>&& t) noexcept;
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
inline constexpr TupleElement_t<I,U>& Get_impl(TupleElement<I, U>& te) noexcept
{
        return te.Get();
}

template<std::size_t I, typename U>
inline constexpr TupleElement_t<I,U>const& Get_impl(TupleElement<I, U> const& te) noexcept
{
        return te.Get();
}

template<std::size_t I, typename U>
inline constexpr TupleElement_t<I,U>&& Get_impl(TupleElement<I, U>&& te) noexcept
{
        return std::move(te).Get();
}

template<std::size_t I, typename T, typename... Types>
inline constexpr decltype(auto) Get(Tuple_impl<T, Types...>& t) noexcept
{
    return Get_impl<I>(t);
}
template<std::size_t I, typename T, typename... Types>
inline constexpr decltype(auto) Get(Tuple_impl<T, Types...> const& t) noexcept
{
    return Get_impl<I>(t);
}
template<std::size_t I, typename T, typename... Types>
inline constexpr decltype(auto) Get(Tuple_impl<T, Types...>&& t) noexcept
{
    return Get_impl<I>(std::move(t));
}


// Comparison operations
/* --------------------------------------------------------------------------------------------- */
constexpr inline bool operator==(Tuple<> const&, Tuple<> const&)
{
    return true;    // empty tuples are always equal
}

template<std::size_t... Is, typename... Ts, typename... Us>
constexpr inline bool TupleEquals_impl(Tuple<Ts...> const& lhs, Tuple<Us...> const& rhs,
                      std::index_sequence<Is...>) noexcept
{
    return (... && (Get<Is>(lhs) == Get<Is>(rhs)));
}

template<typename... Ts, typename... Us
    /* ,typename = std::enable_if_t<sizeof...(Ts) == sizeof...(Us)> */>
constexpr inline bool operator==(Tuple<Ts...> const& lhs, Tuple<Us...> const& rhs) noexcept
{
    if constexpr (sizeof...(Ts) != sizeof...(Us)) {
        return false;
    }
    return TupleEquals_impl(lhs, rhs, std::index_sequence_for<Ts...>{});
}
/* --------------------------------------------------------------------------------------------- */

// !=
/* --------------------------------------------------------------------------------------------- */
constexpr inline bool operator!=(Tuple<> const&, Tuple<> const&)
{
    return false;
}

template<typename... Ts, typename... Us>
constexpr inline bool operator!=(Tuple<Ts...> const& lhs, Tuple<Us...> const& rhs) noexcept
{
    if constexpr (sizeof...(Ts) != sizeof...(Us)) {
        return true;
    }
    else {
        return !TupleEquals_impl(lhs, rhs, std::index_sequence_for<Ts...>{});
    }
}
/* --------------------------------------------------------------------------------------------- */


// <
/* --------------------------------------------------------------------------------------------- */
constexpr inline bool operator<(Tuple<> const&, Tuple<> const&) noexcept
{
    return false;
}

template<typename> struct SequenceCount_impl;

template<template<typename I, I...>class Seq, typename T, T... Indices>
struct SequenceCount_impl<Seq<T,Indices...>> {
    static constexpr inline auto value = sizeof...(Indices);
};

template<typename Sequence>
struct SequenceCount : SequenceCount_impl<Sequence> { };

template<typename Sequence>
constexpr inline auto SequenceCount_v = SequenceCount<Sequence>::value;

template<typename> struct SequenceFront_impl;

template<template<typename U, U...> class Seq, typename T, T I, T... Indices>
struct SequenceFront_impl<Seq<T,I,Indices...>>
{
    static constexpr inline auto value = I;
};

template<typename Seq>
struct SequenceFront : SequenceFront_impl<Seq> { };

template<typename Seq>
constexpr inline auto SequenceFront_v = SequenceFront<Seq>::value;

template<typename> struct SequencePopFront_impl;

template<template<typename U, U...>class Seq, typename T, T Head, T... Tail>
struct SequencePopFront_impl<Seq<T,Head,Tail...>>
{
    using type = Seq<T,Tail...>;
};

template<typename Seq>
struct SequencePopFront : SequencePopFront_impl<Seq> { };

template<typename Seq>
using SequencePopFront_t = typename SequencePopFront<Seq>::type;

template<typename... Ts, typename TsIdxSeq, typename... Us, typename UsIdxSeq>
constexpr inline bool TupleLT_impl(Tuple<Ts...>const& lhs, TsIdxSeq,
                                   Tuple<Us...>const& rhs, UsIdxSeq) noexcept
{
    if constexpr (SequenceCount_v<TsIdxSeq> != 0 && SequenceCount_v<UsIdxSeq> != 0) {
        const auto left = Get<SequenceFront_v<TsIdxSeq>>(lhs);
        const auto right = Get<SequenceFront_v<UsIdxSeq>>(rhs);
        if (left == right) {
            return TupleLT_impl(lhs, SequencePopFront_t<TsIdxSeq>{},
                                rhs, SequencePopFront_t<UsIdxSeq>{});
        }
        return left < right;
    }
    else {
        const auto Ts_idx_count = SequenceCount_v<TsIdxSeq>;
        const auto Us_idx_count = SequenceCount_v<UsIdxSeq>;
        if (Ts_idx_count == 0 && Us_idx_count != 0) {
            return true;
        }
        return false;
    }
}

template<typename... Ts, typename... Us>
constexpr inline bool operator<(Tuple<Ts...> const& lhs, Tuple<Us...> const& rhs) noexcept
{
    return TupleLT_impl(lhs, std::index_sequence_for<Ts...>{},
                        rhs, std::index_sequence_for<Us...>{});
}
/* --------------------------------------------------------------------------------------------- */
