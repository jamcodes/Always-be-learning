#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <string_view>

class Foo
{
public:
    template<std::size_t N>
    constexpr std::tuple_element_t<N,Foo> get() const { return get_impl<N>(*this); }

    template<std::size_t N>
    constexpr std::tuple_element_t<N,Foo> get() { return get_impl<N>(*this); }
private:
    template<std::size_t N, typename T>
    static constexpr auto get_impl(T& self) -> decltype(self.template get<N>())
    {
        static_assert(0 <= N && N < 3, "Foo has only 3 elements");
        if constexpr (N == 0) { return self.i_; }
        else if constexpr (N == 1) { return self.d_; }
        else if constexpr (N == 2) { return self.str_; }
    }
    int i_{42};
    double d_{3.14};
    std::string str_{"hello bindings"};
};

namespace std
{
    template<> struct tuple_size<Foo> : public std::integral_constant<size_t, 3> { };

    template<> struct tuple_element<0,Foo> { using type = int; };
    template<> struct tuple_element<1,Foo> { using type = double; };
    template<> struct tuple_element<2,Foo> { using type = string&; };
} // namespace std


int main()
{
    Foo f;

    auto [i, d, s] = f;

    const Foo cf;
    auto [a, b, c] = cf;
}
