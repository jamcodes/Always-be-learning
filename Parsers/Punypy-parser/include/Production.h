#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>

#include "debug.h"


class PunyPyWorld;

class Bad_production : public std::logic_error
{
    using std::logic_error::logic_error;
};

struct Params_tag { };
static constexpr Params_tag params_tag{};

class Prod_base
{
public:
    virtual ~Prod_base() noexcept = default;
protected:
    friend class Production;
    friend class Expression;
    Prod_base() noexcept = default;
    Prod_base(const Prod_base&) noexcept = default;
    Prod_base(Prod_base&&) noexcept = default;
    Prod_base& operator=(const Prod_base&) noexcept = default;
    Prod_base& operator=(Prod_base&&) noexcept = default;

    virtual Prod_base* clone() const& = 0;
    virtual Prod_base* clone() && = 0;

    // virtual void eval() const noexcept = 0;
    virtual std::string rep() const = 0;
    virtual void analyze(PunyPyWorld&) = 0;

    virtual int interpret(PunyPyWorld&)
    {
        INFO();
        return 0;
    }

    // virtual int value() const
    // {
    //     throw Bad_production{"Value called on a non Int_prod production"};
    // }

    // virtual size_t param_count() const
    // {
    //     throw Bad_production{
    //         "param_count() called on a non Param_prod/FuncCal_prod/FuncDef_prod production"
    //         };
    // }


    // virtual std::vector<int> interpret(Params_tag, PunyPyWorld&)
    // {
    //     throw Bad_production{"Bad interpret call"};
    // }
};


struct Funcdef_tag { };
static constexpr Funcdef_tag funcdef_tag{};
struct Funccall_tag { };
static constexpr Funccall_tag funccall_tag{};
struct Expr_tag { };
static constexpr Expr_tag expr_tag{};
struct Funcbod_tag { };
static constexpr Funcbod_tag funcbod_tag{};
struct Declaration_tag { };
static constexpr Declaration_tag declaration_tag{};
struct Eof_tag { };
static constexpr Eof_tag eof_tag{};

class Expression;
class Parameters;

class Production
{
public:
    explicit Production(Eof_tag);
    explicit Production(Expression&&) noexcept;
    Production(Funcdef_tag, std::string name, Parameters&& params, Production&& body);
    Production(Funcbod_tag, std::string indent, std::vector<Production>&& body);
    Production(Funccall_tag, std::string name, Parameters&& params);
    Production(Declaration_tag, std::string name, Expression&& val);

    ~Production() noexcept = default;
    Production(Production&&) noexcept = default;
    Production& operator=(Production&&) noexcept = default;
    Production(const Production& other)
        : prod_{other.clone()} { }
    Production& operator=(const Production& other)
    {
        auto copy = other.clone();
        prod_ = std::move(copy);
        return *this;
    }
    // Support cloning, should be private with all Prod_base derived classes declared friends
    Production(std::unique_ptr<Prod_base> prod) noexcept
        : prod_{std::move(prod)} { }
    
    Production(const Prod_base& prod)
        : prod_{prod.clone()} { }

    std::unique_ptr<Prod_base> clone() const &
    {
        return std::unique_ptr<Prod_base>{prod_->clone()};
    }

    std::unique_ptr<Prod_base> clone() &&
    {
        return std::unique_ptr<Prod_base>{std::move(prod_)->clone()};
    }

    std::string rep() const
    {
        return prod_->rep();
    }

    void analyze(PunyPyWorld& world)
    {
        prod_->analyze(world);
    }

    int interpret(PunyPyWorld& world)
    {
        return prod_->interpret(world);
    }

    // int value() const
    // {
    //     return prod_->value();
    // }

    // size_t param_count() const
    // {
    //     return prod_->param_count();
    // }


    // std::vector<int> interpret(Params_tag tag, PunyPyWorld& world)
    // {
    //     return prod_->interpret(tag, world);
    // }

friend std::ostream& operator<<(std::ostream&, const Production&);
private:
    std::unique_ptr<Prod_base> prod_;
};

class Expr_prod : public Prod_base
{
    friend class Expression;
public:
    virtual ~Expr_prod() noexcept = default;
protected:
    // friend class Production;
    friend class Expression;
    Expr_prod() noexcept = default;
    Expr_prod(const Expr_prod&) noexcept = default;
    Expr_prod(Expr_prod&&) noexcept = default;
    Expr_prod& operator=(const Expr_prod&) noexcept = default;
    Expr_prod& operator=(Expr_prod&&) noexcept = default;

    Expr_prod* clone() const& override = 0;
    Expr_prod* clone() && override = 0;

    virtual int value() const
    {
        throw Bad_production{"Value called on a non Int_prod production"};
    }

    virtual const std::string& name() const
    {
        throw Bad_production("name called on a non Var_prod production");
    }
};

class Expression
{
public:
    friend class Production;
    explicit Expression(int i); // Int_prod
    explicit Expression(std::string); // Var_prod
    // Plus_prod
    Expression(int a, int b);
    Expression(std::string a, std::string b);
    Expression(int a, std::string b);
    Expression(std::string a, int b);
    Expression(Expression&&, Expression&&);

    ~Expression() noexcept = default;
    Expression(Expression&&) noexcept = default;
    Expression& operator=(Expression&&) noexcept = default;
    Expression(const Expression& other)
        : expr_{other.clone()}
        { }
    Expression& operator=(const Expression& other)
    {
        auto copy = other.clone();
        expr_ = std::move(copy);
        return *this;
    }

    // Support cloning, should be private with all Prod_base derived classes declared friends
    Expression(std::unique_ptr<Expr_prod> expr) noexcept
        : expr_{std::move(expr)} { }

    std::unique_ptr<Expr_prod> clone() const &
    {
        return std::unique_ptr<Expr_prod>{expr_->clone()};
    }

    std::unique_ptr<Expr_prod> clone() &&
    {
        return std::unique_ptr<Expr_prod>{std::move(expr_)->clone()};
    }

        std::string rep() const
    {
        return expr_->rep();
    }

    void analyze(PunyPyWorld& world)
    {
        expr_->analyze(world);
    }

    int value() const
    {
        return expr_->value();
    }

    const std::string& name() const
    {
        return expr_->name();
    }

    int interpret(PunyPyWorld& world)
    {
        return expr_->interpret(world);
    }

    // size_t param_count() const
    // {
    //     return expr_->param_count();
    // }


    // std::vector<int> interpret(Params_tag tag, PunyPyWorld& world)
    // {
    //     return expr_->interpret(tag, world);
    // }

private:
    std::unique_ptr<Expr_prod> expr_;
};

// struct Parameterable
// {
//     // virtual ~Parameterable() noexcept = default;
//     // friend class Production;
//     // friend class Expression;
//     // Parameterable() noexcept = default;
//     // Parameterable(const Parameterable&) noexcept = default;
//     // Parameterable(Parameterable&&) noexcept = default;
//     // Parameterable& operator=(const Parameterable&) noexcept = default;
//     // Parameterable& operator=(Parameterable&&) noexcept = default;
//     virtual size_t param_count() const = 0;
// };

class Params_prod : public Prod_base
{
    friend class Production;
    friend class Parameters;
    friend std::unique_ptr<Params_prod>
        std::make_unique<Params_prod>(std::vector<Expression>&&);
    friend std::unique_ptr<Params_prod>
        std::make_unique<Params_prod>(const Params_prod&);
    friend std::unique_ptr<Params_prod>
        std::make_unique<Params_prod>(Params_prod&&);

    Params_prod(std::vector<Expression>&& vec)
        : params_{std::move(vec)} { }

    Params_prod* clone() const& final
    {
        // return std::make_unique<Params_prod>(*this);
        return new Params_prod{*this};
    }
    Params_prod* clone() && final
    {
        // return std::make_unique<Params_prod>(std::move(*this));
        return new Params_prod{std::move(*this)};
    }

    std::string rep() const final
    {
        auto oss = std::ostringstream{};
        oss << "- Parameters =\n";
        for(const auto& p : params_)
            oss << "    " << p.rep() << "\n";
        return oss.str();
    }

    void analyze(PunyPyWorld& world) final
    {
        INFO();
        for(auto&& p : params_) {
            p.analyze(world);
        }
    }

    const auto& params() const
    {
        return params_;
    }

    size_t param_count() const
    {
        return params_.size();
    }

// --- intentionally shadowing virtual int interpret(PunyPyWorld)
    std::vector<int> interpret(Params_tag, PunyPyWorld& world)
    {
        auto res = std::vector<int>();
        for(auto&& p : params_){
            res.emplace_back(p.interpret(world));
        }
        return res;
    }

// --- member data
    std::vector<Expression> params_; // std::vector<Expression>
};

class Parameters
{
public:
    explicit Parameters(std::vector<Expression>&&);
    ~Parameters() noexcept = default;
    Parameters(Parameters&&) noexcept = default;
    Parameters& operator=(Parameters&&) noexcept = default;
    Parameters(const Parameters& other)
        : params_{other.clone()}
        { }
    Parameters& operator=(const Parameters& other)
    {
        auto copy = other.clone();
        params_ = std::move(copy);
        return *this;
    }
    // Support cloning, should be private with all Prod_base derived classes declared friends
    Parameters(std::unique_ptr<Params_prod> expr) noexcept
        : params_{std::move(expr)} { }

    std::unique_ptr<Params_prod> clone() const &
    {
        return std::unique_ptr<Params_prod>{params_->clone()};
    }

    std::unique_ptr<Params_prod> clone() &&
    {
        return std::unique_ptr<Params_prod>{std::move(params_)->clone()};
    }

        std::string rep() const
    {
        return params_->rep();
    }

    void analyze(PunyPyWorld& world)
    {
        params_->analyze(world);
    }

    const std::vector<Expression>& params() const
    {
        return params_->params();
    }

    size_t param_count() const
    {
        return params_->param_count();
    }

// --- intentionally shadowing virtual int interpret(PunyPyWorld)
    std::vector<int> interpret(/* Params_tag tag,  */PunyPyWorld& world)
    {
        return params_->interpret(params_tag, world);
    }

    // int interpret(PunyPyWorld& world)
    // {
    //     return params_->interpret(world);
    // }

    // int value() const
    // {
    //     return params_->value();
    // }
private:
    std::unique_ptr<Params_prod> params_;
};

class Decl_prod : public Prod_base
{
    friend class Production;
    friend std::unique_ptr<Decl_prod>
        std::make_unique<Decl_prod>(std::string&&, Expression&&);
    friend std::unique_ptr<Decl_prod>
        std::make_unique<Decl_prod>(const Decl_prod&);
    friend std::unique_ptr<Decl_prod>
        std::make_unique<Decl_prod>(Decl_prod&&);

    Decl_prod(std::string&& name, Expression&& val)
        : name_{std::move(name)}, val_{std::move(val)}
        { }

    Decl_prod* clone() const& final
    {
        // return std::make_unique<Decl_prod>(*this);
        return new Decl_prod{*this};
    }
    Decl_prod* clone() && final
    {
        // return std::make_unique<Decl_prod>(std::move(*this));
        return new Decl_prod{std::move(*this)};
    }

    std::string rep() const final
    {
        auto oss = std::ostringstream{};
        oss << "* Declaration\n  - name = " << name_
            << "\n  - value =\n" << val_.rep();
        return oss.str();
    }

    void analyze(PunyPyWorld& world) final;
    int interpret(PunyPyWorld&) final;

    std::string name_;
    // Production  val_; // Expression
    Expression val_;
};

inline auto Declaration(std::string&& name, Expression&& val)
{
    return Production(declaration_tag, std::move(name), std::move(val));
}

class Eof_prod : public Prod_base
{
    friend class Production;
    friend std::unique_ptr<Eof_prod>
        std::make_unique<Eof_prod>();
    friend std::unique_ptr<Eof_prod>
        std::make_unique<Eof_prod>(const Eof_prod&);
    friend std::unique_ptr<Eof_prod>
        std::make_unique<Eof_prod>(Eof_prod&&);

    Eof_prod() noexcept = default;

    Eof_prod* clone() const& final
    {
        // return std::make_unique<Eof_prod>(*this);
        return new Eof_prod{*this};
    }
    Eof_prod* clone() && final
    {
        // return std::make_unique<Eof_prod>(std::move(*this));
        return new Eof_prod{std::move(*this)};
    }

    std::string rep() const final
    {
        return "\nDone.";
    }

    void analyze(PunyPyWorld& /* world */) final
    {
        INFO();
        // return nullptr;
    }

    int interpret(PunyPyWorld& /* world */) final
    {
        return 0;
    }
};

inline auto Eof() noexcept
{
    return Production{eof_tag};
}

class Func_base : public Prod_base
{

};

class FuncDef_prod : public Func_base
{
    friend class Production;
    friend class Function;
    friend std::unique_ptr<FuncDef_prod>
        std::make_unique<FuncDef_prod>(std::string&&, Parameters&&, Production&&);
    friend std::unique_ptr<FuncDef_prod>
        std::make_unique<FuncDef_prod>(const FuncDef_prod&);
    friend std::unique_ptr<FuncDef_prod>
        std::make_unique<FuncDef_prod>(FuncDef_prod&&);

    FuncDef_prod(std::string name, Parameters&& params, Production&& body)
        : name_{std::move(name)}, params_{std::move(params)}, body_{std::move(body)} { }

    FuncDef_prod* clone() const& final
    {
        // return std::make_unique<FuncDef_prod>(*this);
        return new FuncDef_prod{*this};
    }
    FuncDef_prod* clone() && final
    {
        // return std::make_unique<FuncDef_prod>(std::move(*this));
        return new FuncDef_prod{std::move(*this)};
    }

    std::string rep() const final
    {
        auto oss = std::ostringstream{};
        oss << "* Function\n  - name = " << name_ 
            << "\n  " << params_.rep()
            << "\n  " << body_.rep();
        return oss.str();
    }

    void analyze(PunyPyWorld& world) final;

    int interpret(PunyPyWorld&) final;

    const std::vector<Expression>& params() const
    {
        return params_.params();
    }

    size_t param_count() const
    {
        return params_.param_count();
    }

    std::string name_;
    Parameters params_; // Parameters
    Production body_; // Function_body
};

inline auto Funcdef(std::string&& name, Parameters&& params, Production&& body) -> Production
{
    return Production{funcdef_tag, std::move(name), std::move(params), std::move(body)};
}

class Funcbod_prod : public Prod_base
{
    friend class Production;
    friend std::unique_ptr<Funcbod_prod>
        std::make_unique<Funcbod_prod>(std::string&&, std::vector<Production>&&);
    friend std::unique_ptr<Funcbod_prod>
        std::make_unique<Funcbod_prod>(const Funcbod_prod&);
    friend std::unique_ptr<Funcbod_prod>
        std::make_unique<Funcbod_prod>(Funcbod_prod&&);

    Funcbod_prod(std::string indent, std::vector<Production>&& body)
        : indent_{std::move(indent)}, body_{std::move(body)} { }

    Funcbod_prod* clone() const& final
    {
        // return std::make_unique<Funcbod_prod>(*this);
        return new Funcbod_prod{*this};
    }
    Funcbod_prod* clone() && final
    {
        // return std::make_unique<Funcbod_prod>(std::move(*this));
        return new Funcbod_prod{std::move(*this)};
    }

    std::string rep() const final
    {
        auto oss = std::ostringstream{};
        oss << "- function body";
        for(const auto& p : body_)
            oss << "\n    " << indent_ << p.rep();
        return oss.str();
    }

    void analyze(PunyPyWorld& world) final
    {
        INFO();
        for(auto&& p : body_) {
            p.analyze(world);
        }
    }

    int interpret(PunyPyWorld& world) final
    {
        auto res = int{};
        for(auto&& p : body_){
            res = p.interpret(world);
        }
        return res;
    }


    std::string indent_;
    std::vector<Production> body_; // OK
};

inline auto Function_body(std::string&& indent, std::vector<Production>&& body) -> Production
{
    return Production{funcbod_tag, std::move(indent), std::move(body)};
}

class FuncCall_prod :public Prod_base
{
    friend class Production;
    friend std::unique_ptr<FuncCall_prod>
        std::make_unique<FuncCall_prod>(std::string&&, Parameters&&);
    friend std::unique_ptr<FuncCall_prod>
        std::make_unique<FuncCall_prod>(const FuncCall_prod&);
    friend std::unique_ptr<FuncCall_prod>
        std::make_unique<FuncCall_prod>(FuncCall_prod&&);

    FuncCall_prod(std::string name, Parameters&& params)
        : name_{std::move(name)}, params_{std::move(params)} { }

    FuncCall_prod* clone() const& final
    {
        // return std::make_unique<FuncCall_prod>(*this);
        return new FuncCall_prod{*this};
    }
    FuncCall_prod* clone() && final
    {
        // return std::make_unique<FuncCall_prod>(std::move(*this));
        return new FuncCall_prod{std::move(*this)};
    }

    std::string rep() const final
    {
        auto oss = std::ostringstream{};
        oss << "* Call:\n  - name = " << name_ 
            << "\n  " << params_.rep();
        return oss.str();
    }

    void analyze(PunyPyWorld& world) final;

    int interpret(PunyPyWorld&) final;

    size_t param_count() const
    {
        return params_.param_count();
    }

    std::string name_;
    Parameters params_; // Parameters
};

inline auto Funccall(std::string&& name, Parameters&& params) -> Production
{
    return Production{funccall_tag, std::move(name), std::move(params)};
}

class Function
{
public:
    explicit Function(const FuncDef_prod& func)
        : func_{func.clone()} { }
    ~Function() noexcept = default;
    Function(Function&&) noexcept = default;
    Function& operator=(Function&&) noexcept = default;
    Function(const Function& other)
        : func_{other.clone()}
        { }
    Function& operator=(const Function& other)
    {
        auto copy = other.clone();
        func_ = std::move(copy);
        return *this;
    }

    std::unique_ptr<FuncDef_prod> clone() const &
    {
        return std::unique_ptr<FuncDef_prod>{func_->clone()};
    }

    std::unique_ptr<FuncDef_prod> clone() &&
    {
        return std::unique_ptr<FuncDef_prod>{std::move(func_)->clone()};
    }

        std::string rep() const
    {
        return func_->rep();
    }

    void analyze(PunyPyWorld& world)
    {
        func_->analyze(world);
    }

    // int interpret(PunyPyWorld& world)
    // {
    //     return func_->interpret(world);
    // }

    size_t param_count() const
    {
        return func_->param_count();
    }

    const std::vector<Expression>& params() const
    {
        return func_->params();
    }

    int call(PunyPyWorld& world)
    {
        return func_->body_.interpret(world);
    }

    // int value() const
    // {
    //     return func_->value();
    // }

private:
    std::unique_ptr<FuncDef_prod> func_;
};


class Int_prod : public Expr_prod
{
    friend class Production;
    friend class Plus_prod;
    friend std::unique_ptr<Int_prod>
        std::make_unique<Int_prod>(int&&);
    friend std::unique_ptr<Int_prod>
        std::make_unique<Int_prod>(const Int_prod&);
    friend std::unique_ptr<Int_prod>
        std::make_unique<Int_prod>(Int_prod&&);

    explicit Int_prod(int i)
        : val_{i} { }

    Expr_prod* clone() const& final
    {
        // return std::make_unique<Int_prod>(*this);
        return new Int_prod{*this};
    }
    Expr_prod* clone() && final
    {
        // return std::make_unique<Int_prod>(std::move(*this));
        return new Int_prod{std::move(*this)};
    }

    std::string rep() const final
    {
        auto oss = std::ostringstream{};
        oss << "    Int: " << val_;
        return oss.str();
    }

    void analyze(PunyPyWorld& /* world */) final
    {
        INFO();
        // Nothing to do?
    }

    int value() const final { return val_; }

    int interpret(PunyPyWorld& /*world*/) final
    {
        return val_;
    }

// --- member data
    int val_;
};

class Var_prod : public Expr_prod
{
    friend class Production;
    friend std::unique_ptr<Var_prod>
        std::make_unique<Var_prod>(std::string&&);
    friend std::unique_ptr<Var_prod>
        std::make_unique<Var_prod>(const Var_prod&);
    friend std::unique_ptr<Var_prod>
        std::make_unique<Var_prod>(Var_prod&&);

    explicit Var_prod(std::string name)
        : var_{std::move(name)} { }

    Var_prod* clone() const& final
    {
        // return std::make_unique<Var_prod>(*this);
        return new Var_prod{*this};
    }
    Var_prod* clone() && final
    {
        // return std::make_unique<Var_prod>(std::move(*this));
        return new Var_prod{std::move(*this)};
    }

    std::string rep() const final
    {
        auto oss = std::ostringstream{};
        oss << "    Var: " << var_;
        return oss.str();
    }

    const std::string& name() const
    {
        return var_;
    }

    void analyze(PunyPyWorld& world) final;

    int interpret(PunyPyWorld& world) final;

// --- member data
    std::string var_;
};

class Plus_prod : public Expr_prod
{
    friend class Production;
    friend std::unique_ptr<Plus_prod>
        std::make_unique<Plus_prod>(int&, int&);
    friend std::unique_ptr<Plus_prod>
        std::make_unique<Plus_prod>(int&, std::string&&);
    friend std::unique_ptr<Plus_prod>
        std::make_unique<Plus_prod>(std::string&&, int&);
    friend std::unique_ptr<Plus_prod>
        std::make_unique<Plus_prod>(std::string&&, std::string&&);
    friend std::unique_ptr<Plus_prod>
        std::make_unique<Plus_prod>(Expression&&, Expression&&);
    friend std::unique_ptr<Plus_prod>
        std::make_unique<Plus_prod>(const Plus_prod&);
    friend std::unique_ptr<Plus_prod>
        std::make_unique<Plus_prod>(Plus_prod&&);

    Plus_prod(int a, int b)
        : left_{a}, right_{b} { }
    Plus_prod(std::string a, std::string b)
        : left_{std::move(a)}, right_{std::move(b)} { }
    Plus_prod(int a, std::string b)
        : left_{a}, right_{std::move(b)} { }
    Plus_prod(std::string a, int b)
        : left_{std::move(a)}, right_{b} { }
    Plus_prod(Expression&& lhs, Expression&& rhs)
        : left_{std::move(lhs)}, right_{std::move(rhs)} { }

    Plus_prod* clone() const& final
    {
        // return std::make_unique<Plus_prod>(*this);
        return new Plus_prod{*this};
    }
    Plus_prod* clone() && final
    {
        // return std::make_unique<Plus_prod>(std::move(*this));
        return new Plus_prod{std::move(*this)};
    }

    std::string rep() const final
    {
        auto oss = std::ostringstream{};
        oss << "  - Plus\n    " << left_.rep() << "\n    " << right_.rep();
        return oss.str();
    }

    void analyze(PunyPyWorld& world) final
    {
        INFO();
        left_.analyze(world);
        right_.analyze(world);
    }

    int interpret(PunyPyWorld& world) final
    {
        return left_.interpret(world) + right_.interpret(world);
    }

// --- member data
    Expression left_; // Expression
    Expression right_; // Expression
};

template<typename T
//    ,typename = std::enable_if_t<
//     std::is_integral_v<std::decay_t<T>> ||
//     std::is_convertible_v<std::decay_t<T>,std::string>>
>
inline auto make_expression(T&& val) -> Expression
{
    static_assert(
        std::is_integral_v<std::decay_t<T>> ||
        std::is_convertible_v<std::decay_t<T>,std::string>,
        "Must be called with either an int or a type convertible to std::string"
    );
    return Expression{std::forward<T>(val)};
}
