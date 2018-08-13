#include <iostream>
#include <vector>
#include <queue>
#include <memory>


template<typename Derived>
class Base
{

public:

    void do_something() const noexcept
    {
        static_cast<const Derived*>(this)->do_something_impl();
    }

    void do_something_else() const noexcept
    {
        static_cast<const Derived*>(this)->do_something_else_impl();
    }

private:
    void do_something_impl() const noexcept
    {
        std::cout << "Base: Hello default implementation!" << std::endl;
    }

    void do_something_else_impl() const noexcept
    {
        std::cout << "Base: doing something else..." << std::endl;
    }
};


class Foo : public Base<Foo>
{
    friend class Base<Foo>;
private:
    void do_something_impl() const noexcept
    {
        std::cout << "Foo: Hello!" << std::endl;
    }

    void do_something_else_impl() const noexcept
    {
        std::cout << "Foo: doing something else..." << std::endl;
    }
};

class Bar : public Base<Bar>
{ }; // default implementation of do_something_impl

class Baz : public Base<Baz>
{
private:
    friend class Base<Baz>;
    int data;
private:
    void do_something_else_impl() const noexcept
    {
        std::cout << "Baz: doing something else... " << data << std::endl;
    }

public:
    Baz() : data{0} { }
    Baz(int data_) : data{data_} { }
};


template<typename Derived>
void use(const Base<Derived>& obj)
{
    std::cout << "use: " << std::endl;
    obj.do_something();
    obj.do_something_else();
}

template<typename Derived>
void usep(Base<Derived>* const ptr)
{
    std::cout << "usep: " << std::endl;
    ptr->do_something();
    ptr->do_something_else();
}

template<typename Derived>
void use_shared_ptr(const std::shared_ptr<Base<Derived>>& sptr)
{
    std::cout << "use_shared_ptr: " << std::endl;
    sptr->do_something();
    sptr->do_something_else();
}

template<typename Derived>
void use_unique_ptr(const std::unique_ptr<Base<Derived>> uptr)
{
    std::cout << "use_unique_ptr: " << std::endl;
    uptr->do_something();
    uptr->do_something_else();
}

template<typename Derived, typename... Args>
void insert(std::vector<std::shared_ptr<Base<Derived>>>& v, Args&&... args)
{
    v.push_back( static_cast<std::shared_ptr<Base<Derived>>>(
                    std::make_shared<Derived>(std::forward<Args>(args)...)));
}


int main()
{

    Foo f;
    Bar b;
    Baz bz;
    auto sptr_foo = static_cast<std::shared_ptr<Base<Foo>>>(std::make_shared<Foo>());

    use(f);
    use(b);
    use(bz);
    usep(&f);
    usep(&b);
    usep(&bz);
    use_shared_ptr(sptr_foo);
    use_shared_ptr(static_cast<std::shared_ptr<Base<Bar>>>(std::make_shared<Bar>()));
    use_unique_ptr(static_cast<std::unique_ptr<Base<Foo>>>(std::make_unique<Foo>()));
    use_unique_ptr(static_cast<std::unique_ptr<Base<Bar>>>(std::make_unique<Bar>()));


// Kind of pointless... might aswell have a vector<Baz>...
    std::cout << "\nvector of shared_ptr<Base<Baz>>:" << std::endl;

    std::vector<std::shared_ptr<Base<Baz>>> vba;
    for(unsigned i=0; i<10; ++i){
        insert(vba, i);
    }

    for( const auto& a : vba ){
        a->do_something();
        a->do_something_else();
    }

}
