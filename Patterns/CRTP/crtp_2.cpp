#include <iostream>


template<typename T>
class NumericalFunctions
{
public:
    void scale( double factor )
    {
        T& underlying = static_cast<T&>(*this);
        underlying.set_value(underlying.get_value() * factor);
    }

    void square()
    {
        T& underlying = static_cast<T&>(*this);
        underlying.set_value(underlying.get_value() * underlying.get_value());
    }

    void set_to_opposite()
    {
        scale(-1);
    }
private:
// --- We can declare the constructor private
// and the derived class T a friend in order to catch any potential instantiation
// mistakes like -- class A : public Base<NOT_A> {...};
// at compiletime. Class will only be construcable if it inherits from
// base of itself not base of another class.
    NumericalFunctions() noexcept = default;
    friend T;
};


class Sensitivity : public NumericalFunctions<Sensitivity>
{
public:
    Sensitivity() noexcept = default;
    explicit Sensitivity( double value ) noexcept : m_value{value} { };
    double get_value() const noexcept { return m_value; }
    void set_value(double value) noexcept { m_value = value; }
private:
// --- member data
    double m_value{0};
};


int main()
{
    auto sens = Sensitivity(42);
    std::cout << "sens = " << sens.get_value() << std::endl;
    sens.scale(0.42);
    std::cout << "sens.scale(0.42) = " << sens.get_value() << std::endl;
    sens.square();
    std::cout << "sens.square() = " << sens.get_value() << std::endl;
    sens.set_to_opposite();
    std::cout << "sens.set_to_oposite() = " << sens.get_value() << std::endl;
}
