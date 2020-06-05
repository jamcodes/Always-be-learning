#include <iostream>
#include <string>

#include "new_tracker.h"

struct Foo {
    float* p{new float[100]};
};

struct Bar {
    int five{5};
    std::string s{"hello"};
};

int main()
{
    NewTracker::reset();
    NewTracker::trace(true);
    int* my_int{new int{1337}};
    double* my_double{new double{3.14}};
    double* my_double_array{new double[2]{1.1, 1.2}};

    Foo* foo{new Foo};
    Bar* bar{new Bar};

    // leak my_int
    delete my_double;
    delete[] my_double_array;
    delete foo;
    delete bar;

    NewTracker::status();
}
