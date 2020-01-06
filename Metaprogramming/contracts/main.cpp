#include <iostream>
#include <exception>
#include <string>

using namespace std::string_literals;

#include "contract_assert.h"

constexpr unsigned ModuleContractLevel{3};  // should normally be configured by the build system

struct CerrHandler : jam::ContractLevel<ModuleContractLevel> {
    static void handle(jam::source_location const& loc, const char* const expr)
    {
        std::cerr << "Contract violated -- " << loc.file << ":" << loc.line << ", expression = \"" << expr
                  << "\"\n";
    }
};

struct ThrowHandler : jam::ContractLevel<ModuleContractLevel> {
    static void handle(jam::source_location const& loc, const char* const expr)
    {
        std::string msg{"Contract violated -- "};
        msg.reserve(100);
        msg += loc.file;
        msg += ":";
        msg += std::to_string(loc.line);
        msg += ", expression = \"";
        msg += expr;
        msg += "\"";
        throw std::logic_error{std::move(msg)};
    }
};

struct TerminateHandler : jam::ContractLevel<ModuleContractLevel> {
    [[noreturn]] static void handle(jam::source_location const& loc, const char* const expr) noexcept
    {
        std::cerr << "Contract violated -- " << loc.file << ":" << loc.line << ", expression = \"" << expr
                  << "\"\n";
        std::terminate();
    }
};

void foo(int* i) noexcept
{
    CONTRACT_ASSERTION(i != nullptr, CerrHandler{}, jam::ContractLevel<2>{});
    *i *= 42;
}

void bar(int* i)
{
    CONTRACT_ASSERTION(i != nullptr, ThrowHandler{}, jam::ContractLevel<2>{});
    *i /= 42;
}

int baz(int i) noexcept
{
    CONTRACT_ASSERTION(i != 0, TerminateHandler{}, jam::ContractLevel<2>{});
    return 42 / i;
}

int main()
{
    int a{11};
    try {
        bar(&a);
        bar(nullptr);
    }
    catch (std::logic_error const& e) {
        std::cerr << "logic_error: " << e.what() << "\n";
    }

    foo(&a);
    foo(nullptr);

    baz(42);
    baz(0);
}
