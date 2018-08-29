#pragma once


#include <istream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <variant>
#include <stdexcept>

#include "debug.h"
#include "Token.h"
#include "Scanner.h"
#include "Production.h"

/**
 * PunyPy grammar
 *
 * root        := (Funccall | Funcdef | Declaration)*
 * Funcdef     := DEF Name LPAREN Params RPAREN COLON Body
 * Funccal     := Name LPAREN Params RPAREN
 * Declaration := Name EQUALS Expression
 * Params      := Expression (COMMAA Expression)*
 * Expression  := Name | Plus | Integer
 * Body        := (INDENT Expression | Funccal)*
 * Plus        := Expression PLUS Expression
 * Integer     := (0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9)+
 * EQUALS      := "="
 * PLUS        := "+"
 * LPAREN      := "("
 * RPAREN      := ")"
 * COLON       := ":"
 * COMMA       := ","
 * DEF         := "def"
 * INDENT      := "    " ; some amount of whitespace at the start of a line
 *
 * TODO:
 * Statement
 *      := (Funccall | Funcdef)
 *       | (Funccall | Funcdef) Statement
 *
 * Funcdef
 *      := DEF Name LPAREN Params RPAREN COLON Body
 *
 * Funccall
 *      := name LPAREN params RPAREN
 * 
 * Params
 *      := Expression
 *       | Expression COMMA expression
 *
 * Expression
 *      := Primary
 *       | Expression PLUS Primary
 *       | Expression MINUS Primary
 *
 * Primary 
 *      := Integer
 *       | Name
 *       | PLUS Primary
 *       | MINUS Primary
 *       | LPAREN Expression RPAREN
 * 
 * PLUS   = "+"
 * MINUS  = "-"
 * LPAREN = "("
 * RPAREN = ")"
 * COLON  = ":"
 * COMMA  = ","
 * DEF    = "def"
 */


class Bad_token : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

/**
 * \brief 'PunyPy' Parser used to parse an extremely simplified python grammar
 */
class Parser
{
    using scanner_t = Scanner;
public:
    explicit Parser(std::istream& is)
        : scanner_{is}
        { }

    auto root() -> Production;
    auto function_def() -> Production;
    auto function_call(std::string&& name) -> Production;
    auto declaration(std::string&& name) -> Production;
    auto function_body() -> Production;
    auto parameters() -> Parameters;
    auto expression() -> Expression;
    auto expression(std::string&&) -> Expression;
    auto plus(Expression&&) -> Expression;

    explicit operator bool() const
    {
        return scanner_.operator bool();
    }

private:
    scanner_t scanner_;
};
