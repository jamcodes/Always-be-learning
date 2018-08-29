#include <iostream>
#include <utility>
#include <sstream>
#include "Scanner.h"


Token Scanner::get()
{
    if(full_){
        full_ = false;
        return std::move(buffer_);
    }

    char ch;
    source_.get(ch);
    if(source_.eof()) {
        return Token(Kind::Eof);
    }

    switch(ch)
    {
    case '(':
    case ')':
    case '+':
    case '-':
    case ':':
    case ',':
    case '=':
        return Token{ch};
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        {
            source_.putback(ch);
            int val;
            source_ >> val;
            return Token{val};
        }
    case '\n':
        {
            while(source_.get(ch) && ch == '\n') // don't care about vertical whitespace
                ;
            if (!source_)
            {
                return Token{Kind::Eof};
            }
            if (std::isspace(ch))
            {
                auto oss = std::ostringstream{};
                oss << ch;
                while (source_.get(ch) && std::isspace(ch))
                {
                    oss << ch;
                }
                source_.putback(ch);
                return Token{Kind::Indent, oss.str()};
            }
            else
            {
                source_.putback(ch);
                return get();
            }
        }
    case std::istream::traits_type::eof():
        std::cerr << "Reached EOF" << std::endl;
        return Token{Kind::Eof};
    default:
        {
            if(std::isalpha(ch))
            {
                auto oss = std::ostringstream{};
                oss << ch;
                while(source_.get(ch) && (std::isalpha(ch) || isdigit(ch) || ch == '_'))
                    oss << ch;
                source_.putback(ch);
                if(oss.str() == Def)
                    return Token{Kind::Def};
                return Token{oss.str()}; // Kind::Name
            }
            if(std::isspace(ch))
            {
                while((ch = source_.peek()) != '\n' && std::isspace(ch))
                    source_.ignore();
                return get();
            }
            throw std::runtime_error("Invalid Token");
        }
    } // switch
}

Token Scanner::peek()
{
    if(full_){
        return buffer_;
    }
    const auto ret = get();
    full_ = true;
    buffer_ = ret;
    return ret;
}

void Scanner::putback(const Token& t)
{
    if(full_){
        throw std::logic_error("putback into a full buffer");
    }
    buffer_ = t;
    full_ = true;
}

void Scanner::putback(Token&& t)
{
    if(full_){
        throw std::logic_error("putback into a full buffer");
    }
    buffer_ = std::move(t);
    full_ = true;
}

void Scanner::ignore(const Token& t)
{
    if(full_ && t == buffer_){
        full_ = false;
        return;
    }
    full_ = false;

    for(Token tok = get(); tok != t; tok = get())
        ;
}

void Scanner::ignore(const Kind k)
{
    if(full_ && buffer_.kind() == k){
        full_ = false;
        return;
    }
    full_ = false;
    for(Token t = get(); t.kind() != k; t = get())
        ;
}
