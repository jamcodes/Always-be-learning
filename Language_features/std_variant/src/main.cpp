#include <variant>
#include <utility>
#include <iostream>
#include <string>
#include <vector>
#include <type_traits>

using namespace std::string_literals;

class MyVisitor
{
  public:
    constexpr explicit MyVisitor(std::ostream& os)
        : os_{os} {}

    void operator()(int i) noexcept
    {
        os_ << "MyVisitor: int value = " << i << "\n";
    }

    void operator()(const char* cstr) noexcept
    {
        os_ << "MyVisitor: const char* value = " << cstr << "\n";
    }

    void operator()(const std::string& str) noexcept
    {
        os_ << "MyVisitor: std::string value = " << str << "\n";
    }

  private:
    std::ostream& os_;
};

template<typename T>
struct always_false : std::false_type { };
template<typename T>
inline constexpr auto always_false_v = always_false<T>::value;


// Variadic overload type for inheriting from lambdas...
template<typename... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};
// template deduction guide
template<typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

// Go a step further - provide a nicer interface
template<typename Variant, typename... Ts>
decltype(auto) match(Variant&& v, Ts&&... ts)
{
    return std::visit(
        overloaded{std::forward<Ts>(ts)...},
        std::forward<Variant>(v)
        );
}


int main()
{
    using var_t = std::variant<int, const char*, std::string>;

    auto values = std::vector<var_t>{};

    // constructing / assigning values
    values.push_back(10);
    // construct in-place by index of the type
    values.emplace_back(std::in_place_index_t<1>{}, "Hello there!");
    // construct in-place by type
    values.emplace_back(std::in_place_type_t<std::string>{}, "Wow!");

    // construct in-place
    auto val = var_t{std::in_place_type_t<const char*>{}, "Some value"};
    // assign in-place by type index
    val.emplace<0>(42);
    // assign in-place by type
    val.emplace<std::string>("A different value");

// If an empty-state is needed, or all the type alternative are non-default-constructible
// the std::monostate can be used to indicate the empty-state, or 'uninitialized'
    std::cout << "\nMonostate example:\n";
    std::variant<std::monostate, std::string> monoval{};
    if (std::holds_alternative<std::monostate>(monoval)) {
        std::cout << "monoval holds the monostate" << "\n";
    }
    monoval.emplace<std::string>("Not a monostate anymore");
    std::cout << "get<std::string>(monoval) = " << std::get<std::string>(monoval) << "\n";

    // reading values
    // use std::get<> with either type or type index
    auto &s = std::get<std::string>(values.back()); // throws if not std::string
    s += " That is great!";
    std::cout << s << "\n";

    // use std::holds_alternative to check if the given type is stored
    if (std::holds_alternative<const char*>(values[1]))
    {
        std::cout << "values[1] is of type 'const char*'\n";
    }

    // use std::get_if to obtain a pointer to the held value,
    // or nullptr if variant doesn't currently hold the requested type
    const auto* const pi = std::get_if<int>(&val);
    if (!pi) {
        std::cout << "std::get_if<int>(&val) returned a nullptr - val doesn't hold an int\n";
    }

    // use visitor pattern - std::visit
    // generic lambda to generate call operators for all types
    std::cout << "\nGeneric lambda:\n";
    for (const auto& v : values)
    {
        std::visit([](auto&& arg) { std::cout << arg << "\n"; }, v);
    }
    std::visit([](auto&& arg) { std::cout << arg << '\n'; }, val);

    // use a custom object with operator() overloads for all possible types held by the variant
    std::cout << "\nCustom defined visitor:\n";
    for (const auto& v : values)
    {
        std::visit(MyVisitor{std::cout}, v);
    }
    std::visit(MyVisitor{std::cout}, val);

    // use a generic lambda, but also utilise `if constexpr` to, at compile time, identify the type
    // of the variable held by the variant. This is an alternative to a class with overloaded
    // operator()'s
    std::cout << "\nType matching lambda visitor...\n";
    auto type_matching_lambda_visitor = [](auto&& arg){
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int>) {
                std::cout << "type-matching visitor: int value = " << arg << "\n";
            }
            else if constexpr (std::is_same_v<T, const char*>) {
                std::cout << "type-matching visitor: const char* value = " << arg << "\n";
            }
            else if constexpr (std::is_same_v<T, std::string>) {
                std::cout << "type-matching visitor: std::string value = " << arg << "\n";
            }
            else {
                static_assert(always_false_v<T>, "Non-exhaustive visitor!");
            }
        };
    for (const auto& v : values)
    {
        std::visit(type_matching_lambda_visitor, v);
    }
    std::visit(type_matching_lambda_visitor, val);

    // we can also inherit from lambdas:
    std::cout << "\nLambda-inheriting visitor...\n";
    for (const auto& v : values)
    {
        std::visit(overloaded{
            [](auto&& arg) { std::cout << "generic overload: " << arg << "\n"; },
            [](int i) { std::cout << "int overload: " << i << "\n"; },
            [](const char* cstr) { std::cout << "const char* overload: " << cstr << "\n"; }
        }, v);
    }

    // using the `match` template
    std::cout << "\nLambda-inheriting visitor via `match` convenience template...\n";
    for (const auto& v : values)
    {
        match(v,
            [](auto&& arg) { std::cout << "generic match: " << arg << "\n"; },
            [](int i) { std::cout << "int match: " << i << "\n"; },
            [](const char* cstr) { std::cout << "const char* match: " << cstr << "\n"; }
        );
    }
}
