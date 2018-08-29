#include <iostream>

#include "Parser.h"
#include "Production.h"


auto Parser::root() -> Production
{
    switch(auto token = scanner_.peek(); token.kind())
    {
    case Kind::Def:
        scanner_.ignore(Kind::Def);
        return function_def();
    case Kind::Name:
        scanner_.ignore(Kind::Name);
        if(scanner_.peek().kind() == Kind::LParen){
            return function_call(token.str_val());
        }
        // Kind::Equals
        return declaration(token.str_val());
    case Kind::Eof:
        return Eof();
    default:
        throw Bad_token("Function definition or function call expected");
    }
}

auto Parser::function_def() -> Production
{
    const auto name = scanner_.get();
    if(name.kind() != Kind::Name){
        throw Bad_token("Invalid token, Name expected");
    }
    auto tok = scanner_.get();
    if(tok.kind() != Kind::LParen){
        throw Bad_token("Invalid token, \"(\" expected");
    }
    auto params = parameters();
    tok = scanner_.get();
    if(tok.kind() != Kind::RParen){
        throw Bad_token("Invalid token, \")\" expected");
    }
    tok = scanner_.get();
    if(tok.kind() != Kind::Colon){
        throw Bad_token("Invalid token, \":\" expected");
    }
    auto body = function_body();
    return Funcdef(name.str_val(), std::move(params), std::move(body));
}

auto Parser::function_call(std::string&& name) -> Production
{
    // auto tok = scanner_.get();
    // if(tok.kind() != Kind::Name){
    //     throw Bad_token("Invalid token, Name expected");
    // }
    // auto name = tok.str_val();

    auto tok = scanner_.get();
    if(tok.kind() != Kind::LParen){
        throw Bad_token("Invalid token, \"(\" expected");
    }
    auto params = parameters();
    tok = scanner_.get();
    if(tok.kind() != Kind::RParen){
        throw Bad_token("Invalid token, \")\" expected");
    }
    return Funccall(std::move(name), std::move(params));
}

auto Parser::declaration(std::string&& name) -> Production
{
    auto tok = scanner_.get();
    if(tok.kind() != Kind::Equals){
        throw Bad_token(R"_(Invalid token, ")" expected)_");
    }
    return Declaration(std::move(name), expression());
}

auto Parser::function_body() -> Production
{
    auto tok = scanner_.get();
    if(tok.kind() != Kind::Indent){
        throw Bad_token("Invalid token, Indent expected");
    }
    auto indent = tok.str_val();
    auto body = std::vector<Production>();
    while(true)
    {
        tok = scanner_.peek();
        switch(tok.kind())
        {
        case Kind::Int:
            body.emplace_back(Production{expression()});
            break;
        case Kind::Name:
            // funccall or expression, assume funccall for now
            // auto name = tok.str_val();
            scanner_.ignore(Kind::Name);
            if(scanner_.peek().kind() != Kind::LParen){
                body.emplace_back(expression(tok.str_val()));
            }
            else{
                body.emplace_back(function_call(tok.str_val()));
            }
            break;
        default:
            throw Bad_token("Invalid token, Name or Int expected");
        }
        tok = scanner_.peek();
        if(tok.kind() != Kind::Indent){
            return Function_body(std::move(indent), std::move(body));
        }
        scanner_.ignore(Kind::Indent);
    }
    throw Bad_token("Empty function body");
}

auto Parser::parameters() -> Parameters
{
    auto params = std::vector<Expression>();
    for(auto tok = scanner_.peek(); tok.kind() != Kind::RParen;)
    {
        params.emplace_back(expression());
        tok = scanner_.peek();
        switch(tok.kind())
        {
        case Kind::Comma:
            scanner_.ignore(Kind::Comma);
            break;
        case Kind::RParen:
            return Parameters{std::move(params)};
        default:
            throw Bad_token("Invalid token, \",\" or \")\" expected");
        }
    }
    return Parameters{std::move(params)};
}

auto Parser::expression() -> Expression
{
    switch(auto tok = scanner_.get(); tok.kind())
    {
    case Kind::Name:
        // if(scanner_.peek().kind() == Kind::Plus){
        //     return plus(Production{tok.str_val()});
        // }
        // return Expression(tok.str_val());
        return expression(tok.str_val());
    case Kind::Int:
        if(scanner_.peek().kind() == Kind::Plus){
            return plus(Expression{tok.int_val()});
        }
        return Expression{tok.int_val()};
    default:
        throw Bad_token("Invalid token, 'Name' or 'Int' expected");
    }
}

auto Parser::expression(std::string&& name) -> Expression
{
    if(scanner_.peek().kind() == Kind::Plus){
        return plus(Expression{std::move(name)});
    }
    return Expression{std::move(name)};
}

auto Parser::plus(Expression&& left) -> Expression
{
    auto tok = scanner_.get();
    if(tok.kind() != Kind::Plus){
        throw Bad_token("Invalid token, \"+\" expected");
    }
    auto right = expression();
    return Expression{std::move(left), std::move(right)};
}
