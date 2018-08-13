#include <iostream>


// convenience crtp class to avoid explicit static casts
template<typename T, template<typename> typename crtpType>
struct crtp
{
    T& underlying() { return static_cast<T&>(*this); }
    const T& underlying() const { return static_cast<const T&>(*this); }
private:
    crtp() noexcept = default;
    friend crtpType<T>;
};

template<typename T>
struct Scale : public crtp<T, Scale>
{
    void scale(double factor)
    {
        this->underlying().set_value(this->underlying().get_value() * factor);
    }
};

template<typename T>
struct Square : public crtp<T, Square>
{
    void square()
    {
        this->underlying().set_value(this->underlying().get_value() * this->underlying().get_value());
    }

    double get_squared() const
    {
        auto rv = this->underlying().get_value();
        return rv * rv;
    }
};


class Sensitivity : public Scale<Sensitivity>, public Square<Sensitivity>
{
public:
    explicit Sensitivity(double val) noexcept
        : value_{val} { }
    double get_value() const { return value_; }
    void set_value( double value ) { value_ = value; }
private:
    double value_;
};


int main()
{
    auto sens = Sensitivity(42);
    std::cout << "sens = " << sens.get_value() << std::endl;
    sens.scale(0.42);
    std::cout << "sens.scale(0.42) = " << sens.get_value() << std::endl;
    sens.square();
    std::cout << "sens.square() = " << sens.get_value() << std::endl;

    const auto csens = Sensitivity(42);
    std::cout << "csens.get_squared() = " << csens.get_squared() << std::endl;
    // won't compile
    // csens.square();
}
