#include <iostream>
#include <utility>

template<typename Derived>
class Base {
public:

     void action() const
     {
        derived().action();
     }

    Base() = default;
    Base(const Base&) = default;
    Base(Base&&) noexcept = default;
    Base& operator=(Base const&) = default;
    Base& operator=(Base&&) noexcept = default;

protected:
    ~Base() = default;
    friend Derived;

private:
    Derived& derived() & noexcept { return static_cast<Derived&>(*this); }
    Derived const& derived() const& noexcept { return static_cast<Derived const&>(*this); }
    Derived&& derived() && noexcept { return static_cast<Derived&&>(*this); }
    Derived const&& derived() const&& noexcept { return static_cast<Derived const&&>(*this); }
};

template<typename D = void>
class Middle : public Base<Middle<D>>{
public:
    void action() const
    {
        // action_impl(std::is_same<D, void>{});

        // in C++17 - do without the helper
        if constexpr (std::is_same<D, void>::value) {
            std::cerr << "Middle action\n";
        }
        else {
            static_cast<D const*>(this)->action();
        }
    }

private:
    // Middle type constructed directly - not a call through an inheriting type
    // void action_impl(std::true_type) const
    // {
    //     std::cerr << "Middle\n";
    // }

    // void action_impl(std::false_type) const
    // {
    //     static_cast<D const*>(this)->action();
    // }
};

class Final : public Middle<Final>{
public:
    void action() const
    {
        std::cerr << "Final action\n";
    }
};

template<typename T>
void through_base(Base<T> const& b)
{
    std::cerr << __PRETTY_FUNCTION__ << "\n";
    b.action();
}

template<typename T>
void through_middle(Middle<T> const& m)
{
    std::cerr << __PRETTY_FUNCTION__ << "\n";
    m.action();
}

int main()
{
    Middle m;
    std::cerr << "Middle object calls:\n";
    through_base(m);
    through_middle(m);

    Final f;
    std::cerr << "\nFinal object calls:\n";
    through_base(f);
    through_middle(f);
}
