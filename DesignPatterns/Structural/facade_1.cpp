#include <iostream>
#include <string>
#include <memory>
#include <utility>

/**
 * The Facade design pattern provides a unified interface to a set of interfaces in a subsystem.
 * It defines a simpler, higher-level interface that manages multiple subsystems and makes them
 * easier to use.
 * It wraps a complicated class or class hierarchy in a class with a simpler interface.
 */

class SubSystemOne {
public:
    void MethodOne() { std::cerr << "SubSystemOne::MethodOne\n"; }
};

class SubSystemTwo {
public:
    void MethodTwo() { std::cerr << "SubSystemTwo::MethodTwo\n"; }
};

class SubSystemThree {
public:
    void MethodThree() { std::cerr << "SubSystemThree::MethodThree\n"; }
};

// Facade
class Facade {
public:
    void method_a()
    {
        std::cerr << "Facade::method_a\n";
        p_one_->MethodOne();
        p_two_->MethodTwo();
    }

    void method_b()
    {
        std::cerr << "Facade::method_b\n";
        p_two_->MethodTwo();
        p_three_->MethodThree();
    }

private:
// Assume that the subsystems are polymorphic hierarchies and we need to dynamically allocate
    std::unique_ptr<SubSystemOne> p_one_{std::make_unique<SubSystemOne>()};
    std::unique_ptr<SubSystemTwo> p_two_{std::make_unique<SubSystemTwo>()};
    std::unique_ptr<SubSystemThree> p_three_{std::make_unique<SubSystemThree>()};
};


int main()
{
    Facade f{};
    f.method_a();
    f.method_b();
}
