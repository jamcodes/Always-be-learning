#include <iostream>

// There are four main "class natures" (the reason to implement a class):
// * Value types
// * Polymorphic types
// * Relationship (pointer/references) types
// * Manager types
// Cannonical forms of these class natures are the patterns that should be applied to guide
// safe implementation of these types

// # Value types
// Preffer aggregates and Rule of Zero - can't be bettered
// Otherwise - If a class with private data needs to be implemented for a value type, consider
// the modified Rule of Six (if can't apply Rule of Zero):
class ValueGood {
    ValueGood() = default;
    ~ValueGood() = default;
    ValueGood(const ValueGood&) = default;
    ValueGood(ValueGood&&) noexcept = default;
    ValueGood& operator=(ValueGood const&) & = default;     // Note the `&` qualifier
    ValueGood& operator=(ValueGood&&) & noexcept = default; // same
};
// Ref-qualifying the assignment operators makes it impossible to assign to a temporary
// which may increase the code safety whenever assigning to a temporary is not within the intended
// use case scope (it rarely is).

// # Polymorphic
// OO base classes need to have a virtual destructor, and prohibited copying to prevent slicing.
// Apply the Rule of Six when implementing polymorphic class hierarchies or consider the possibility
// of deleting only the move assignment operator.
// When the move assignment operator is explicitly deleted both the move constructor and the copy
// costructor and assignment operator are implicitly deleted. The default constructor is still
// available. This is the least amount of code to get the desired behavior.
class OOBaseSix {
    OOBaseSix() = default;
    virtual ~OOBaseSix() = default;
    OOBaseSix(OOBaseSix const&) = delete;
    OOBaseSix(OOBaseSix&&) noexcept = delete;
    OOBaseSix& operator=(OOBaseSix const&) = delete;
    OOBaseSix& operator=(OOBaseSix&&) noexcept = delete;
};

class OOBase {
    virtual ~OOBase() = default;
    OOBase& operator=(OOBase&&) = delete;
};
// OOBase achieves the same as OOBaseSix with significantly less code.


int main()
{

}
