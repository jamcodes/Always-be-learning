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

/* token_char - template representing single-character literals                                  */
/* --------------------------------------------------------------------------------------------- */
template<char Ch>
using token_char = std::integral_constant<char, Ch>;
/* --------------------------------------------------------------------------------------------- */

/* teken_literal - template representing string literals                                         */
/* --------------------------------------------------------------------------------------------- */
template<typename T>
struct token_literal : T
{
    static_assert(has_value_v<T>, "token_literal type needs to define a 'value' constant");
    using type = token_literal;
    using value_type = get_value_type_t<std::decay_t<decltype(T::value)>>;
    constexpr operator value_type() const noexcept { return T::value; };
    constexpr value_type operator()() const noexcept { return T::value; };

    template<typename L, typename R> friend constexpr
    auto operator==(const token_literal<L>&/*lhs*/, const token_literal<R>&/*rhs*/) noexcept;

    template<typename L, typename R> friend constexpr
    auto operator!=(const token_literal<L>&/*lhs*/, const token_literal<R>&/*rhs*/) noexcept;

    template<typename L, typename R> friend constexpr
    auto operator<(const token_literal<L>&/*lhs*/, const token_literal<R>&/*rhs*/) noexcept;

    template<typename L, typename R> friend constexpr
    auto operator>(const token_literal<L>&/*lhs*/, const token_literal<R>&/*rhs*/) noexcept;

    template<typename L, typename R> friend constexpr
    auto operator<=(const token_literal<L>&/*lhs*/, const token_literal<R>&/*rhs*/) noexcept;

    template<typename L, typename R> friend constexpr
    auto operator>=(const token_literal<L>&/*lhs*/, const token_literal<R>&/*rhs*/) noexcept;
};

// These must be declared in-class and defined outside of class.
// Defining inline in-class causes an ODR error when unstantiating multiple templates.
template<typename L, typename R> constexpr inline
auto operator==(const token_literal<L>&/*lhs*/, const token_literal<R>&/*rhs*/) noexcept
{ return L::value == R::value; }

template<typename L, typename R> constexpr inline
auto operator!=(const token_literal<L>&/*lhs*/, const token_literal<R>&/*rhs*/) noexcept
{ return L::value != R::value; }

template<typename L, typename R> constexpr inline
auto operator<(const token_literal<L>&/*lhs*/, const token_literal<R>&/*rhs*/) noexcept
{ return L::value < R::value; }

template<typename L, typename R> constexpr inline
auto operator>(const token_literal<L>&/*lhs*/, const token_literal<R>&/*rhs*/) noexcept
{ return L::value > R::value; }

template<typename L, typename R> constexpr inline
auto operator<=(const token_literal<L>&/*lhs*/, const token_literal<R>&/*rhs*/) noexcept
{ return L::value <= R::value; }

template<typename L, typename R> constexpr inline
auto operator>=(const token_literal<L>&/*lhs*/, const token_literal<R>&/*rhs*/) noexcept
{ return L::value >= R::value; }
/* --------------------------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------------------------- */
template<typename T>
class token_variable
{
public:
    using value_type = std::remove_reference_t<std::remove_cv_t<T>>;
    using return_type = get_value_type_t<value_type>;
    template<typename U, typename =
        std::enable_if_t<std::is_convertible_v<U, value_type> >
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

    operator return_type() const { return value_; }
    return_type operator()() const { return value_; }

    template<typename L, typename R>
    friend auto operator==(const token_variable<L>& lhs, const token_variable<R>& rhs) noexcept;

    template<typename L, typename R>
    friend auto operator!=(const token_variable<L>& lhs, const token_variable<R>& rhs) noexcept;

    template<typename L, typename R>
    friend auto operator<(const token_variable<L>& lhs, const token_variable<R>& rhs) noexcept;

    template<typename L, typename R>
    friend auto operator>(const token_variable<L>& lhs, const token_variable<R>& rhs) noexcept;

    template<typename L, typename R>
    friend auto operator<=(const token_variable<L>& lhs, const token_variable<R>& rhs) noexcept;

    template<typename L, typename R>
    friend auto operator>=(const token_variable<L>& lhs, const token_variable<R>& rhs) noexcept;

private:
    value_type value_;
};

template<typename L, typename R> inline
auto operator==(const token_variable<L>& lhs, const token_variable<R>& rhs) noexcept
{ return lhs.value_ == rhs.value_; }

template<typename L, typename R> inline
auto operator!=(const token_variable<L>& lhs, const token_variable<R>& rhs) noexcept
{ return lhs.value_ != rhs.value_; }

template<typename L, typename R> inline
auto operator<(const token_variable<L>& lhs, const token_variable<R>& rhs) noexcept
{ return lhs.value_ < rhs.value_; }

template<typename L, typename R> inline
auto operator>(const token_variable<L>& lhs, const token_variable<R>& rhs) noexcept
{ return lhs.value_ > rhs.value_; }

template<typename L, typename R> inline
auto operator<=(const token_variable<L>& lhs, const token_variable<R>& rhs) noexcept
{ return lhs.value_ <= rhs.value_; }

template<typename L, typename R> inline
auto operator>=(const token_variable<L>& lhs, const token_variable<R>& rhs) noexcept
{ return lhs.value_ >= rhs.value_; }
/* --------------------------------------------------------------------------------------------- */

using lparen    = token_char<'('>;
using rparen    = token_char<')'>;
using plus      = token_char<'+'>;
using minus     = token_char<'-'>;
using colon     = token_char<':'>;
using comma     = token_char<','>;
using equals    = token_char<'='>;
using eof_token = token_char< -1>;

// template<char Ch>
// auto make_token() noexcept
// {
//     return token_char<Ch>{};
// }

struct def_value { static constexpr inline std::string_view value{"def"}; };
using def = token_literal<def_value>;
static_assert(std::is_same_v<std::string_view, def::value_type>, "unexpected def value_type");
static_assert(std::is_same_v<std::string_view, decltype(def{}())>, "undexpected def() value_type");
using name = token_variable<std::string>;
static_assert(std::is_same_v<name::return_type, std::string_view>, "unexpected return type");
static_assert(std::is_same_v<name::value_type, std::string>, "unexpected value type");
using integer = token_variable<int>;

template<typename T>
using kind = std::in_place_type_t<T>;

using Token = std::variant<
                lparen, rparen, plus, minus, colon,
                comma, equals, def, name, integer, eof_token
                >;
