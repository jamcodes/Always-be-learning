#include <iostream>


// -- CRTP used as a interface
template<typename T>
class Amount
{
public:
    double get_value() const noexcept
    {
        return static_cast<const T&>(*this).get_value();
    }
};


class Constant : public Amount<Constant>
{
public:
    double get_value() const noexcept { return 42; }
};

class Variable : public Amount<Variable>
{
public:
    explicit Variable(double value) : m_value{value} { }
    double get_value() const noexcept { return m_value; }
private:
    double m_value;
};

template<typename T>
void print(const Amount<T>& amount)
{
    std::cout << amount.get_value() << std::endl;
}


int main()
{
    auto constant = Constant();
    auto variable = Variable(3.1416);
    print(constant);
    print(variable);
}
