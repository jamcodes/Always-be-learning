#include <iostream>
#include <type_traits>
#include <boost/type_traits/has_plus_assign.hpp>
#include <boost/type_traits/has_plus.hpp>
#include <boost/type_traits/has_post_increment.hpp>
#include <boost/type_traits/has_pre_increment.hpp>


// need to define correct functional objects first
struct pre_inc {
    template<typename T>
    constexpr void operator()(T& value) const noexcept { 
        ++value;
        std::cout << "\n" << __PRETTY_FUNCTION__ << "\n";
    }
};

struct post_inc {
    template<typename T>
    constexpr void operator()(T& value) const noexcept {
        value++;
        std::cout << "\n" << __PRETTY_FUNCTION__ << "\n";
    }
};

struct plus_assign {
    template<typename T>
    constexpr void operator()(T& value) const noexcept {
        value += T(1);
        std::cout << "\n" << __PRETTY_FUNCTION__ << "\n";
    }
};

struct plus {
    template<typename T>
    constexpr void operator()(T& value) const noexcept {
        value = value + T(1);
        std::cout << "\n" << __PRETTY_FUNCTION__ << "\n";
    }
};


template<typename T>
void some_action(T& value) noexcept
{
    // worst case scenario
    using step_0_t = plus;

    // std::conditional_t<bool,T,U> is like a compiletime if statement
    // if the bool argument is true returns T if false returns U
    using step_1_t = std::conditional_t<
        boost::has_plus_assign<T>::value,
        plus_assign,
        step_0_t
        >;

    using step_2_t = std::conditional_t<
        boost::has_post_increment<T>::value,
        post_inc,
        step_1_t
        >;

    using step_3_t = std::conditional_t<
        boost::has_pre_increment<T>::value,
        pre_inc,
        step_2_t
        >;

    step_3_t{}(value);
}

// in C++17 we can get the same behavior in a more readable way
template<typename T>
void some_action_cpp17(T& value) noexcept
{
    std::cout << "\n" << __PRETTY_FUNCTION__ << "\t";
    if constexpr(boost::has_pre_increment<T>::value) {
        std::cout << "has_pre_increment\n";
        ++value;
    }
    else if constexpr(boost::has_post_increment<T>::value) {
        std::cout << "has_post_increment\n";
        value++;
    }
    else if constexpr(boost::has_plus_assign<T>::value) {
        std::cout << "has_plus_assign\n";
        value += T{1};
    }
    else {
        std::cout << "has_plus\n";
        value = value + T{1};
    }
}

int main()
{
    auto x = 42;
    some_action(x);

    auto y = 27;
    some_action_cpp17(y);
}
