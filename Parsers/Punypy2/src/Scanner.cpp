#include <iostream>
#include <sstream>
#include <utility>

#include "Token.h"
#include "Scanner.h"

#define GSL_THROW_ON_CONTRACT_VIOLATION
#include <gsl/gsl>
#if defined(NDEBUG)
#undef Expects
#undef Ensures
#define Expects(cond)
#define Ensures(cond)
#endif



Token Scanner::get()
{
    if (full_) {
        full_ = false;
        return std::move(buffer_);
    }

    char ch;
    // source_.get(ch);
    source_ >> ch;
    if (source_.eof()) {
        return Token{kind<eof_token>{}};
    }

    switch(ch) {
    case '(' : return Token{kind<lparen>{}};
    case ')' : return Token{kind<rparen>{}};
    case '+' : return Token{kind<plus>{}};
    case '-' : return Token{kind<minus>{}};
    case ':' : return Token{kind<colon>{}};
    case ',' : return Token{kind<comma>{}};
    case '=' : return Token{kind<equals>{}};

    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        {
            source_.putback(ch);
            int val{0};
            source_ >> val;
            return Token{kind<integer>{}, val};
        }
    // case '\n':
    //     {
    //         while(source_.get(ch) && ch == '\n')    // skip vertical whitespace
    //             ;
    //         if (!source_) {
    //             return Token{kind<eof_token>{}};
    //         }
    //         if (std::isspace(ch)) {
    //             auto oss = std::ostringstream{};
    //             oss << ch;
    //             while (source_.get() && std::isspace(ch)) {
    //                 oss << ch;
    //             }
    //             source_.putback(ch);
    //             re
    //         }
    //     }
    default:
        {
            if (std::isalpha(ch)) {
                std::ostringstream oss;
                oss << ch;
                while (source_.get(ch) && (std::isalpha(ch) || std::isdigit(ch) || ch == '_')) {
                    oss << ch;
                }
                source_.putback(ch);
                if (oss.str() == def::value) {
                    return Token{kind<def>{}};
                }
                return Token{kind<name>{}, oss.str()};
            }
            throw std::runtime_error("Invalid Token");
        }
    } // switch
}

Token Scanner::peek()
{
    if (full_) {
        return buffer_;
    }
    buffer_ = get();
    full_ = true;
    return buffer_;
}

void Scanner::putback(const Token& t)
{
    Expects(!full_);
    buffer_ = t;
    full_ = true;
}

void Scanner::putback(Token&& t)
{
    Expects(!full_);
    buffer_ = std::move(t);
    full_ = true;
}

void Scanner::ignore(const Token& t)
{
    if (full_ && buffer_ == t) {
        full_ = false;
        return;
    }
    full_ = false;
    for (auto tok = get(); tok != t; tok = get()) {
        }
}
