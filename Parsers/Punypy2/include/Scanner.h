#pragma once

#include <iosfwd>
#include "Token.h"


template<typename C, typename T> struct contains_impl; // : std::false_type { };

template<template<class...> class C, typename T, typename... Us>
struct contains_impl<C<Us...>, T> : std::disjunction<std::is_same<T, Us>...> { };

template<typename C, typename T>
static constexpr inline auto contains_v{contains_impl<C,T>::value};


class Scanner
{
    using source_t = std::istream;
public:
    constexpr explicit Scanner(source_t& src) noexcept
        : source_{src}
        { }

    Scanner(const Scanner&) = delete;
    Scanner& operator=(const Scanner&) = delete;
    Scanner(Scanner&&) noexcept = default;
    Scanner& operator=(Scanner&&) noexcept = default;

    Token get();
    Token peek();
    void putback(const Token& t);
    void putback(Token&& t);
    void ignore(const Token& t);
    template<typename T> std::enable_if_t<contains_v<Token, std::decay_t<T>>, void>
    ignore(T&& t);

private:
    source_t& source_;
    bool      full_{false};
    Token     buffer_{};
};

template<typename T>
inline std::enable_if_t<contains_v<Token, std::decay_t<T>>, void>
Scanner::ignore(T&& t)
{
    using type = std::decay_t<T>;
    static_assert(contains_v<Token, type>, "Ignore called with an invalid type");
    ignore(Token{std::move(t)});
}
