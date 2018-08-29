#pragma once

#include <string>
#include <variant>
#include <utility>
#include <ostream>
#include <istream>
#include <type_traits>
#include <stdexcept>
#include <cctype>

namespace
{
    inline const auto Def = std::string{"def"};
    constexpr auto LParen = char{'('};
    constexpr auto RParen = char{')'};
    constexpr auto Plus   = char{'+'};
    constexpr auto Minus  = char{'-'};
    constexpr auto Colon  = char{':'};
    constexpr auto Comma  = char{','};
    constexpr auto Equals = char{'='};
}

enum class Kind
{
    Def,
    Name,
    Int,
    LParen,
    RParen,
    Plus,
    Minus,
    Colon,
    Comma,
    Equals,
    Indent,
    Eof
};

inline std::ostream& operator<<(std::ostream& os, const Kind& k)
{
    switch(k)
    {
    case Kind::Def:
        return os << Def;
    case Kind::Name:
        return os << "name";
    case Kind::Int:
        return os << "Int";
    case Kind::LParen:
        return os << "LParen";
    case Kind::RParen:
        return os << "RParen";
    case Kind::Plus:
        return os << "Plus";
    case Kind::Minus:
        return os << "Minus";
    case Kind::Colon:
        return os << "Colon";
    case Kind::Comma:
        return os << "Comma";
    case Kind::Equals:
        return os << "Equals";
    case Kind::Indent:
        return os << "Indent";
    case Kind::Eof:
        return os << "EOF";
    default:
        throw std::logic_error("Unhandled case on Kind output operator");
    }
}

/*
template<typename T>
concept bool String_convertible =
    requires(T t){
        { 
            std::is_convertible_v<
                std::remove_reference_t<std::remove_cv_t<T>>
            ,   std::string
            >
        }
    };
*/
class Token
{
public:
    using variant_t = std::variant<char, int, std::string>;
    explicit Token(Kind kind) noexcept
        : kind_{kind}, value_{0}
        { }

        template <typename T, typename = std::enable_if_t<
                std::is_convertible_v<std::remove_reference_t<std::remove_cv_t<T>>, std::string>>>
        //      requires String_convertible<T>
        explicit Token(Kind kind, T &&val)
            : kind_{kind}, value_{std::string{std::forward<T>(val)}}
        {
        }

        template <typename T, typename = std::enable_if_t<
                std::is_convertible_v<std::remove_reference_t<std::remove_cv_t<T>>, std::string>>>
        //requires String_convertible<T>
        explicit Token(T &&val)
            : kind_{Kind::Name}, value_{std::string{std::forward<T>(val)}}
        { }

    explicit Token(const char ch)
        : kind_{make_kind(ch)}, value_{ch}
        { }

    explicit Token(const int i)
        : kind_{Kind::Int}, value_{i}
        { }

    Token(const Token&) = default;
    Token& operator=(const Token&) = default;
    Token(Token&&) noexcept = default;
    Token& operator=(Token&&) noexcept = default;

    Kind kind() const noexcept { return kind_; }
    int int_val() const noexcept { return std::get<int>(value_); }
    char char_val() const noexcept { return std::get<char>(value_); }
    std::string str_val() const noexcept { return std::get<std::string>(value_); }

friend constexpr bool operator==(const Token& lhs, const Token& rhs) noexcept;
friend constexpr bool operator!=(const Token& lhs, const Token& rhs) noexcept;
friend std::ostream& operator<<(std::ostream& os, const Token& t);

protected:
    static constexpr Kind make_kind(const char ch)
    {
        switch(ch)
        {
        case '(':
            return Kind::LParen;
        case ')':
            return Kind::RParen;
        case '+':
            return Kind::Plus;
        case '-':
            return Kind::Minus;
        case ':':
            return Kind::Colon;
        case ',':
            return Kind::Comma;
        case '=':
            return Kind::Equals;
        default:
            {
                if(std::isspace(ch))
                    return Kind::Indent;
                else
                    throw std::logic_error("Invalid char in make_kind");
            }
        } // switch
    }

private:
    Kind kind_{};
    variant_t value_{};
};

constexpr bool operator==(const Token& lhs, const Token& rhs) noexcept
{
    return lhs.kind_ == rhs.kind_ && lhs.value_ == rhs.value_;
}

constexpr bool operator!=(const Token& lhs, const Token& rhs) noexcept
{
    return !(lhs == rhs);
}

inline std::ostream& operator<<(std::ostream& os, const Token& t)
{
    os << '{' << t.kind_ << ", ";
    switch(t.kind_)
    {
    case Kind::Name:
        os << t.str_val();
        break;
    case Kind::Int:
        os << t.int_val();
        break;
    case Kind::Indent:
        os << t.str_val().size();
        break;
    case Kind::Eof:
        os << "Eof";
        break;
    case Kind::Def:
        break;
    default:
        os << t.char_val();
    }
    return os << '}';
}
