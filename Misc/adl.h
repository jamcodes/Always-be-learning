#pragma once
#include <string>

// ADL - Argument Dependent Lookup - fundamental rules, search order.
// ADL means that a name lookup includes the namespaces of the arguments passed to the function.
// In actuality the argument dependent lookup runs in tandem with outward lexical scope search.

// Outward Lexical Scope Search - searches in the following order:
// - local function scope (if applicable)
// - class scope
// - enclosing class scope
// - base classes (if applicable)
// - namespace scope
// - enclosing namespaces
// - global :: namespace

namespace b {
    void func(std::string const&);  // b::func
namespace internal {
    void func(int);  // b::internal::func
namespace deep {
    void test() {
        std::string s("hello");
        func(s);  // error: finds only b::internal::func(int).
    }
}  // b::internal::deep
}  // b::internal
}  // b

// ADL searches all namespaces associated with a type:
// - set of namespaces of any type that appears as part of the argument type's full name,
//   including template parameter types
// - namespaces of direct and indirect base classes
//
// e.g.:
// type a::A<b::B, c::internal::C*> will produce searches beginning in
// a, b and c::internal namespaces
// (and any other namespaces associated with a::A, b::B, or c::internal::C)

namespace aspace {
    struct A {};
    template <typename T> struct AGeneric {};
    void func(const A&);
    template <typename T> void find_me(const T&);
}  // namespace aspace

namespace bspace {
    typedef aspace::A AliasForA;
    struct B : aspace::A {};
    template <typename T> struct BGeneric {};
    void test() {
        // ok: base class namespace searched.
        func(B());
        // ok: template parameter namespace searched.
        find_me(BGeneric<aspace::A>());
        // ok: template namespace searched.
        find_me(aspace::AGeneric<int>());
    }
}  // namespace bspace
