#include "ebo.h"

#include <iostream>
#include <type_traits>

struct MyEmptyClass {
    explicit MyEmptyClass() { std::cerr << __func__ << "\n"; }
    ~MyEmptyClass() { std::cerr << __func__ << "\n"; }
};

struct Foo {
    using tag_and_value = CompressedPair<MyEmptyClass, int>;
    tag_and_value tag_and_val_{Zero_then_variadic_args_t{}, 42};
};

struct Foo_no_ebo {
    MyEmptyClass empty_{};
    int val_{42};
};

static_assert(sizeof(Foo) == sizeof(int), "Expected sizeof(Foo) to be == sizeof(int)");
static_assert(sizeof(Foo) < sizeof(Foo_no_ebo), "Expected sizeof(Foo_no_ebo) to be < sizeof(Foo)");

struct Bar {
    using tag_and_value = EmptyBaseOptimized<MyEmptyClass, int>;
    // Since EmptyBaseOptimized is an aggregate the first initializer is the `T1` object,
    // or here - the MyEmptyClass type
    tag_and_value tag_and_val_{MyEmptyClass{}, 42};
};

struct Bar_no_ebo
{
    MyEmptyClass empty_{};
    int val_{42};
};

static_assert(sizeof(Bar) == sizeof(int), "Expected sizeof(Bar) to be == sizeof(int)");
static_assert(sizeof(Bar) < sizeof(Bar_no_ebo), "Expected sizeof(Bar_no_ebo) to be < sizeof(Bar)");

int main()
{
    Foo myfoo{};
    Bar bar{};
    std::cerr << "sizeof(Foo) == " << sizeof(Foo) << "\n";
    std::cerr << "sizeof(Foo_no_ebo) == " << sizeof(Foo_no_ebo) << "\n";
    std::cerr << "myfoo.tag_and_val_.myval2_ = " << myfoo.tag_and_val_.myval2_ << "\n";
    std::cerr << "\nsizeof(Bar) == " << sizeof(Bar) << "\n";
    std::cerr << "sizeof(Bar_no_ebo) == " << sizeof(Bar_no_ebo) << "\n";
    std::cerr << "bar.tag_and_val_.val_ " << bar.tag_and_val_.val_ << "\n";
}
