#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>
#include <memory>
#include <utility>
#include <exception>
#include <boost/lexical_cast.hpp>


/**
 * The `Interpreter` pattern provides a way to include language elements in a program
 */


struct Token {
    enum Type { integer, plus, minus, lparen, rparen };

    Type type;
    std::string text;

    explicit Token(Type t, std::string txt)
        : type{t}, text{std::move(txt)}
    {
    }

    friend std::ostream& operator<<(std::ostream& os, Token const& obj)
    {
        return os << "`" << obj.text << "`";
    }
};

std::vector<Token> lex(std::string const& input)
{
    std::vector<Token> result;
    for (int i = 0; i < input.size(); ++i) {
        switch (input[i]) {
        case '+':
            result.push_back(Token{Token::plus, "+"});
            break;
        case '-':
            result.push_back(Token{Token::minus, "-"});
            break;
        case '(':
            result.push_back(Token{Token::lparen, "("});
            break;
        case ')':
            result.push_back(Token{Token::rparen, ")"});
            break;
        default:
            // number
            std::ostringstream buffer;
            buffer << input[i];
            for (int j = i + 1; j < input.size(); ++j) {
                if (std::isdigit(input[j])) {
                    buffer << input[j];
                    ++i;
                }
                else {
                    result.push_back(Token{Token::integer, buffer.str()});
                    break;
                }
            }
        }
    }

    return result;
}

// parsing

struct Element {
    virtual ~Element() = default;
    virtual int eval() const = 0;
};

struct Integer : public Element {
    int value;
    explicit Integer(int const val)
        : value{val}
    {
    }

    int eval() const override { return value; }
};

struct BinaryOperation : public Element {
public:
    enum Type {addition, substraction};
    Type type;
    std::shared_ptr<Element> lhs, rhs;

    int eval() const override
    {
        if (type == addition) {
            return lhs->eval() + rhs->eval();
        }
        return lhs->eval() - rhs->eval();
    }
};

std::shared_ptr<Element> parse(std::vector<Token> const& tokens)
{
    auto result = std::make_unique<BinaryOperation>();
    bool have_lhs = false;
    for (std::size_t i = 0; i < tokens.size(); ++i) {
        auto token = tokens[i];
        switch (token.type) {
        case Token::integer:
        {
            int value = boost::lexical_cast<int>(token.text);
            auto integer = std::make_shared<Integer>(value);
            if (!have_lhs) {
                result->lhs = integer;
                have_lhs = true;
            }
            else {
                result->rhs = integer;
            }
            break;
        }
        case Token::plus:
            result->type = BinaryOperation::addition;
            break;
        case Token::minus:
            result->type = BinaryOperation::substraction;
            break;
        case Token::lparen:
        {
            int j = i;
            for ( ; j < tokens.size(); ++j) {
                if (tokens[j].type == Token::rparen) {
                    break;
                }
            }
            std::vector<Token> subexpression(&tokens[i+1], &tokens[j]);
            auto element = parse(subexpression);
            if (!have_lhs) {
                result->lhs = element;
                have_lhs = true;
            }
            else {
                result->rhs = element;
                i = j;  // advance
            }
            break;
        }
        } // switch
        return result;
    }
}


int main()
{
    std::string input{ "(13-4)-(12+1)" }; // see if you can make nested braces work
    auto tokens = lex(input);

    // let's see the tokens
    for (auto& t : tokens)
        std::cout << t << "   ";
    std::cout << std::endl;

    try {
        auto parsed = parse(tokens);
        std::cout << input << " = " << parsed->eval() << std::endl;
    } 
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}
