#include <iostream>
#include <memory>


class B {
public:
    virtual ~B() noexcept = default;
    virtual void foo() = 0;             // customization point

    template<class T>
    static std::unique_ptr<T> create()  // interface for creating objects
    {
        auto p = std::make_unique<T>();
        p->post_initialize();
        return p;
    }
protected:
    B() noexcept = default;
    void post_initialize()      // to be called right after construction
    {
        foo();  // virtual dispatch is safe - construction done
    }
};

class D : public B {
public:
    void foo() override {
        std::cout << "D::foo()\n";
    }
};

class C : public B {
public:
    void foo() override {
        std::cout << "C::foo()\n";
    }
};


int main()
{
    auto d = B::create<D>();
    auto c = B::create<C>();
}
