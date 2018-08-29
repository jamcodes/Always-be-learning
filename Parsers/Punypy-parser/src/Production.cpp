#include "Production.h"
#include "PunyPyWorld.h"


Production::Production(Eof_tag /*tag*/)
    : prod_{std::make_unique<Eof_prod>()}
{
}

// The `move(i)` has no effect, but avoids the need to declare multiple friend specilizations
// of std::unique_ptr and std::make_unique
Expression::Expression(int i)
    : expr_{std::make_unique<Int_prod>(std::move(i))}
{
}
Expression::Expression(std::string name)
    : expr_{std::make_unique<Var_prod>(std::move(name))}
{
}

Expression::Expression(int a, int b)
    : expr_{std::make_unique<Plus_prod>(a, b)}
{
}
Expression::Expression(std::string a, std::string b)
    : expr_{std::make_unique<Plus_prod>(std::move(a),std::move(b))}
{
}
Expression::Expression(int a, std::string b)
    : expr_{std::make_unique<Plus_prod>(a, std::move(b))}
{
}
Expression::Expression(std::string a, int b)
    : expr_{std::make_unique<Plus_prod>(std::move(a), b)}
{
}
Expression::Expression(Expression&& lhs, Expression&& rhs)
    : expr_{std::make_unique<Plus_prod>(std::move(lhs), std::move(rhs))}
{
}

Parameters::Parameters(std::vector<Expression>&& params)
    : params_{std::make_unique<Params_prod>(std::move(params))}
{
}

Production::Production(Expression&& expr) noexcept
    : prod_{std::move(expr.expr_)}
{
}

Production::Production(
    Funcdef_tag /*tag*/, std::string name, Parameters&& params, Production&& body
    )
    : prod_{std::make_unique<FuncDef_prod>(std::move(name),std::move(params),std::move(body))}
{
}
Production::Production(Funcbod_tag /*tag*/, std::string indent, std::vector<Production>&& body)
    : prod_{std::make_unique<Funcbod_prod>(std::move(indent), std::move(body))}
{
}
Production::Production(Funccall_tag /*tag*/, std::string name, Parameters&& params)
    : prod_{std::make_unique<FuncCall_prod>(std::move(name), std::move(params))}
{
}

Production::Production(Declaration_tag /*tag*/, std::string name, Expression&& val)
    : prod_{std::make_unique<Decl_prod>(std::move(name), std::move(val))}
{
}

std::ostream& operator<<(std::ostream& os, const Production& p)
{
    return os << p.rep();
}

void Decl_prod::analyze(PunyPyWorld& world)
{
    INFO();
    val_.analyze(world);
    world.set_var(name_, val_.value());
}

int Decl_prod::interpret(PunyPyWorld& world)
{
    return val_.interpret(world);
}

void FuncDef_prod::analyze(PunyPyWorld& world)
{
    INFO();
    // make sure the parameters are all variables
    // params_.analyze(world);
    body_.analyze(world);
    world.set_func(name_, Function{*this});
}

int FuncDef_prod::interpret(PunyPyWorld& /*world*/)
{
    INFO();
    return 0;
}

void FuncCall_prod::analyze(PunyPyWorld& world)
{
    INFO();
    params_.analyze(world);
    // check buildins first
    if(const auto f = world.get_buildin(name_); f){
        return;
    }
    const auto* const func = world.get_func(name_);
    if(!func){
        throw Bad_production("Bad function call. Function " + name_ +
                                " has not been declared");
    }
    // * check if number of parameters matches
    if(params_.param_count() != func->param_count()){
        throw Bad_production(
            "Bad function call, invalid number of parameters."
            "\nGot: " + std::to_string(params_.param_count()) +
            ". Expected: " + std::to_string(func->param_count()));
    }
}

int FuncCall_prod::interpret(PunyPyWorld& world)
{
    // first check if a buildin function exists
    if(auto buildin = world.get_buildin(name_); buildin){
        return buildin(params_.interpret(world));
    }
    // * introduce local scope
    auto local_world = world;
    auto* const function = world.get_func(name_);
    auto params = params_.interpret(world);
    const auto& fparams = function->params();
    auto fp = fparams.cbegin();
    auto p = params.cbegin();
    // * assign all local variables to their respective parameter values
    while(p != params.cend() && fp != fparams.cend()){
        local_world.set_var(fp->name(), *p);
        ++p;
        ++fp;
    }
    // * interpret all expressions within the body
    // * return -> value of the last expression?
    return function->call(local_world);
}

void Var_prod::analyze(PunyPyWorld& world)
{
    INFO();
    auto res = world.get_var(var_);
    if(!res.second){
        throw Bad_production{"Invalid use of a variable. Variable " + var_ +
                                " has not been declared"};
    }
}

int Var_prod::interpret(PunyPyWorld& world)
{
    return world.get_var(var_).first;
}
