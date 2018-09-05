#pragma once

#include <string>
#include <string_view>
#include <variant>
#include <utility>
#include <type_traits>

template<typename T, typename = void>
struct has_value : std::false_type { };
template<typename T>
struct has_value<T, std::void_t<decltype(T::value)>> : std::true_type { };
template<typename T>
static constexpr inline auto has_value_v = has_value<T>::value;

struct bla { };
struct blabla { static constexpr inline const char* const value{"foo"}; };

static_assert(has_value_v<bla> == false, "bla should be false");
static_assert(has_value_v<blabla>, "blabla shoudl be true");
static_assert(std::is_same_v<const char*, std::decay_t<decltype(blabla::value)>>);

template<typename T>
struct get_value_type { using type = T; };
template<> struct get_value_type<const char*> {using type = std::string_view; };
template<> struct get_value_type<std::string> {using type = std::string_view; };
template<typename T>
using get_value_type_t = typename get_value_type<T>::type;


template<char Ch>
using token_char = std::integral_constant<char, Ch>;

template<typename T>
struct token_literal : T
{
    static_assert(has_value_v<T>, "token_literal type needs to define a 'value' constant");
    using value_type = get_value_type_t<std::decay_t<decltype(T::value)>>;
    constexpr operator value_type() const noexcept { return T::value; };
    constexpr value_type operator()() const noexcept { return T::value; };

    template<typename L, typename R>
    friend auto operator==(const token_literal<L>&/*lhs*/, const token_literal<R>&/*rhs*/) noexcept
    { return L::value == R::value; }

    template<typename L, typename R>
    friend auto operator!=(const token_literal<L>&/*lhs*/, const token_literal<R>&/*rhs*/) noexcept
    { return L::value != R::value; }
};

template<typename T>
class token_variable
{
public:
    using value_type = std::decay_t<T>;
    template<typename U, typename =
        std::enable_if_t<std::is_convertible_v<U, value_type> &&
            !std::is_same_v<std::decay_t<U>, token_variable> >
        >
    explicit token_variable(U&& val) noexcept(std::is_nothrow_constructible_v<value_type, U>)
        : value_{std::forward<U>(val)} { }

    token_variable() noexcept(std::is_nothrow_default_constructible_v<value_type>) = default;
    token_variable(const token_variable&) = default;
    token_variable(token_variable&&) noexcept(
        std::is_nothrow_move_constructible_v<value_type>) = default;
    token_variable& operator=(const token_variable&) = default;
    token_variable& operator=(token_variable&&) noexcept(
        std::is_nothrow_move_assignable_v<value_type>) = default;
    ~token_variable() noexcept = default;

    operator value_type() const { return value_; }
    value_type operator()() const { return value_; }

private:
    value_type value_;
};

template<>
class token_variable<std::string> {
public:
    using value_type = std::string;

    template<typename T, typename =
        std::enable_if_t<std::is_convertible_v<std::decay_t<T>, std::string> &&
                         !std::is_same_v<std::decay_t<T>, token_variable> >
        >
    explicit token_variable(T&& val) noexcept(std::is_nothrow_constructible_v<value_type, T>)
        : value_{std::forward<T>(val)} { }

    token_variable(const token_variable&) = default;
    token_variable(token_variable&&) noexcept(
        std::is_nothrow_move_constructible_v<value_type>) = default;
    token_variable& operator=(const token_variable&) = default;
    token_variable& operator=(token_variable&&) noexcept(
        std::is_nothrow_move_assignable_v<value_type>) = default;
    ~token_variable() noexcept = default;

    std::string_view operator()() const noexcept { return value_; }
    operator std::string_view() const noexcept { return value_; }

private:
    value_type value_;
};

using lparen = token_char<'('>;
using rparen = token_char<')'>;
using plus   = token_char<'+'>;
using minus  = token_char<'-'>;
using colon  = token_char<':'>;
using comma  = token_char<','>;
using equals = token_char<'='>;

struct def_value { static constexpr inline std::string_view value{"def"}; };
using def = token_literal<def_value>;
using name = token_variable<std::string>;
using integer = token_variable<int>;

using Token = std::variant<lparen, rparen, plus, minus, colon, comma, equals, def, name, integer>;
