/*
 * Simple calculator program
 * This program implements basic expression calculator.
 * Input from cin output to cout.
 * 
 * Grammar for input:
 * 
 * Calculation:
 *     Statement
 *     Print
 *     Quit
 *     Calculation Statement
 * Statement:
 *     Declaration
 *     Expression
 * Print:
 *     ;
 * Quit:
 *     quit
 * Declaration:
 *     "let" Name "=" Expression
 * Expression:
 *     Term
 *     Expression + Term
 *     Expression - Term
 * Term:
 *     Primary
 *     Term * Priamry
 *     Term / Primary
 *     Term % Primary
 * Primary:
 *     Number
 *     Name
 *     ( Expression )
 *     - Primary
 *     + Primary
 * Number:
 *     floating-point-literal
 * 
 * 
 * Input comes from cin through the Token_stream class object;*/
#include <iostream>
#include <sstream>
#include <cmath>
#include <string>
#include <map>
#include <stdexcept>
#include <memory>
#include <utility>


using namespace std;

namespace{
    constexpr char Print = ';';
    const std::string Quit = "quit";
    const std::string declkey( "let" );
    const std::string prompt( "> ");
    const std::string result("= ");
}

enum class TokenKind : unsigned char
{ Number, Quit, Print, Name, Let
, OpenParen = '(', CloseParen = ')'
, Plus = '+', Minus = '-', Multiply = '*'
, Div = '/', Modulus = '%', Equals = '=' 
};

class Token
{
public:
    Token( TokenKind kind )
        : kind_(kind), value_(0) { }
    Token( double value ) 
        : kind_(TokenKind::Number), value_(value) { }
    Token( const std::string& n )
        : kind_(TokenKind::Name), name_(n) { }
    Token( const Token& t )
        : kind_(t.kind_) { copyUnion(t); }
    Token( Token&& t )
        : kind_(t.kind_) { moveUnion(std::move(t)); }
    Token& operator=( const Token& );
    Token& operator=( Token&& ) noexcept;
    ~Token()
        { if( kind_ == TokenKind::Name ) name_.~string(); }
/* 
    Token& operator=( const std::string& );
    Token& operator=( std::string&& ) noexcept;
    Token& operator=( double );
    Token& operator=( TokenKind ); */

    TokenKind kind() const noexcept { return kind_; }
    const std::string& name() const noexcept { return name_; }
    double value() const noexcept { return value_; }

    bool operator == ( const Token& rhs ) const noexcept;
    bool operator != ( const Token& rhs ) const noexcept;

    bool operator == ( const TokenKind rhs ) const noexcept
        { return kind_ == rhs; }

    // friend TokenStream& operator>>( TokenStream&, Token& t );
private:
    TokenKind kind_;
    union{
        double value_;
        std::string name_;
    };

    void copyUnion( const Token& );
    void moveUnion( Token&& ) noexcept;
};


void Token::copyUnion( const Token& t )
{
    switch(t.kind_){
    case TokenKind::Number:
        value_ = t.value_; break;
    case TokenKind::Name:
        new(&name_) std::string(t.name_); break;
    default:
        /* nothing to copy */
        break;
    }
}

void Token::moveUnion( Token&& t ) noexcept
{
    switch(t.kind_){
    case TokenKind::Number:
        value_ = t.value_; break;
    case TokenKind::Name:
        new(&name_) std::string( std::move(t.name_) ); break;
    default:
        /* nothing to move */
        break;
    }
}

Token& Token::operator=( const Token& rhs )
{
    if( kind_ == TokenKind::Name && rhs.kind_ != TokenKind::Name )
        name_.~string();
    if( kind_ == TokenKind::Name && rhs.kind_ == TokenKind::Name )
        name_ = rhs.name_;
    else
        copyUnion(rhs);
    kind_ = rhs.kind_;
    return *this;
}

Token& Token::operator=( Token&& rhs ) noexcept
{
    if( kind_ == TokenKind::Name && rhs.kind_ != TokenKind::Name )
        name_.~string();
    if( kind_ == TokenKind::Name && rhs.kind_ == TokenKind::Name )
        name_ = std::move( rhs.name_ );
    else
        moveUnion(std::move(rhs));
    kind_ = rhs.kind_;
    return *this;
}


inline bool Token::operator == ( const Token& rhs ) const noexcept
{
    if( kind_ == TokenKind::Number )
        return kind_ == rhs.kind_ && value_ == rhs.value_;
    if( kind_ == TokenKind::Name )
        return kind_ == rhs.kind_ && name_ == rhs.name_;
    return kind_ == rhs.kind_;
}

inline bool Token::operator != ( const Token& rhs ) const noexcept
{
    return !(*this == rhs);
}


class TokenStream
{
public:
    TokenStream( istream& is )
        : source_(is), full_(false), buffer_(0) { }
    TokenStream( const TokenStream& ) = delete;
    TokenStream& operator=( const TokenStream& ) = delete;
    Token get();
    void putback( const Token& t );
    void putback( Token&& t );
    void ignore( const Token& t );
    void ignore( TokenKind );

    explicit operator bool() const noexcept
        { return source_.good() && !source_.fail(); }

    // friend TokenStream& operator>>( TokenStream&, Token& t );
private:
    istream& source_;
    bool full_;
    Token buffer_;
};

Token TokenStream::get()
{
    if( full_ ){
        full_ = false;
        return buffer_;
    }

    char ch;
    source_ >> ch;

    switch( ch ){
    case Print:
        return Token( TokenKind::Print );
    case '(':
    case ')':
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '=':
        return Token( TokenKind(ch) );
    case '.':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    {
        source_.putback(ch);
        double val;
        source_ >> val;
        return Token( val );
    }
    default:
        if( ::isalpha(ch) ){
            std::ostringstream oss;
            oss << ch;
            while(source_.get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_'))
                oss << ch;
            source_.putback(ch);
            std::string s{ std::move(oss.str()) };
            if( oss.str() == declkey ) return Token(TokenKind::Let);
            if( oss.str() == Quit ) return Token(TokenKind::Quit);
            return Token(s);
        }
        throw std::runtime_error("Bad Token");
    }
}

void TokenStream::putback( const Token& t )
{
    if( full_ ) throw std::logic_error( "putback() into a full buffer" );
    buffer_ = t;
    full_ = true;
}

void TokenStream::putback( Token&& t )
{
    if( full_ ) throw std::logic_error( "putback() into a full buffer" );
    buffer_ = std::move(t);
    full_ = true;
}

void TokenStream::ignore( const Token& t )
{
    if( full_ && t == buffer_ ){
        full_ = false;
        return;
    }
    full_ = false;

    // search input
    for( Token tok = get(); tok != t; tok = get() )
        ;
}

void TokenStream::ignore( TokenKind kind )
{
    if( full_ && buffer_ == kind ){
        full_ = false;
        return;
    }
    full_ = false;
    for( Token tok = get(); tok != kind; tok = get() )
        ;
}

class VariableMap
{
public:
    double define_variable( const std::string& name, double val );
    double set_variable( const std::string& name, double val );
    double get_variable( const std::string& name );
private:
// --- member variables
    std::map<std::string,double> varmap_;
};

double VariableMap::define_variable( const std::string& name, double val )
{
    if( varmap_.insert( {name, val} ).second )
        return val;
    throw std::runtime_error( "Redefinition of variable " + name );
}

double VariableMap::set_variable( const std::string& name, double val )
{
    auto it = varmap_.find(name);
    if( it != varmap_.end() ){
        it->second = val;
        return val;
    }
    throw runtime_error( "set: Undefined variable " + name );
}

double VariableMap::get_variable( const std::string& name )
{
    auto it = varmap_.find(name);
    if( it != varmap_.end() ) return it->second;
    throw runtime_error( "get: Undefined variable " + name );
}


class Calculator
{
public:
    Calculator()
        : ts_(cin) { predefined_variables(); }
    Calculator( istream& is )
        : ts_(is) { predefined_variables(); }
    void run();
private:
    double statement();
    double declaration();
    double expression();
    double term();
    double primary();

    void predefined_variables();
// --- member variables
    VariableMap varmap_;
    TokenStream ts_;
};

void Calculator::run()
{
    while( ts_ )
    try{
        cout << prompt;
        Token t = ts_.get();
        while( t == TokenKind::Print ) t = ts_.get();
        if( t == TokenKind::Quit ) return;
        ts_.putback(t);
        cout << result << statement() << endl;
    }
    catch( std::exception& e ){
        std::cerr << e.what() << endl;
        ts_.ignore( TokenKind::Print );
    }
}

double Calculator::statement()
{
    Token t = ts_.get();
    switch( t.kind() ){
    case TokenKind::Let:
        return declaration();
    default:
        ts_.putback(t);
        return expression();
    }
}

double Calculator::declaration()
{
    Token t = ts_.get();
    if( t.kind() != TokenKind::Name )
        throw runtime_error( "Name expected in declaration" );
    std::string var_name = t.name();

    t = ts_.get();
    if( t.kind() != TokenKind::Equals )
        throw runtime_error( "= missing in declaration of " + var_name );
    
    double d = expression();
    varmap_.define_variable( var_name, d );
    return d;
}

double Calculator::expression()
{
    double left = term();

    while( true ){
        Token t = ts_.get();
        switch(t.kind()){
        case TokenKind::Plus:
            left += term();
            // t = ts_.get();
            break;
        case TokenKind::Minus:
            left -= term();
            // t = ts_.get();
            break;
        default:
            ts_.putback(t);
            return left;
        }
    }
}

double Calculator::term()
{
    double left = primary();

    while( true ){
        Token t = ts_.get();
        switch(t.kind()){
        case TokenKind::Multiply :
            left *= primary();
            break;
        case TokenKind::Div :{
                double d = primary();
                if( d == 0 ) throw runtime_error( "divide by zero" );
                left /= d;
                break;
            }
        case TokenKind::Modulus :{
                int i1 = static_cast<int>(left);
                int i2 = static_cast<int>(term());
                if( i2 == 0 ) throw runtime_error( "%: divide by zero" );
                left = i1 % i2;
                break;
            }
        default:
            ts_.putback(t);
            return left;
        }
    }
}

double Calculator::primary()
{
    Token t = ts_.get();
    switch( t.kind() ){
    case TokenKind::OpenParen :{
            double d = expression();
            t = ts_.get();
            if( t.kind() != TokenKind::CloseParen )
                throw runtime_error( "')' expected" );
            return d;
        }
    case TokenKind::Number :
        return t.value();
    case TokenKind::Name :
        return varmap_.get_variable(t.name());
    case TokenKind::Minus :
        return - primary();
    case TokenKind::Plus :
        return primary();
    default:
        throw runtime_error( "primary expected" );
    }
}

void Calculator::predefined_variables()
{
    varmap_.define_variable( "Pi", 3.1415926535 );
    varmap_.define_variable( "e",2.7182818284 );
}

int main()
try{
    Calculator calc;
    calc.run();

    return 0;
}
catch( std::exception& e ){
    cerr << e.what() << endl;
    return 1;
}
catch( ... ){
    cerr << "Unknown exception\n";
    return 2;
}
