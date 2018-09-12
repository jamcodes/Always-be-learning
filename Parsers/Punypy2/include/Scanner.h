#pragma once

#include <iosfwd>
#include "Token.h"


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

private:
    source_t& source_;
    bool      full_{false};
    Token     buffer_{};
};
