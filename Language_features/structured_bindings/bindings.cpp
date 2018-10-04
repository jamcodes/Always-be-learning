#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>
#include <string_view>
#include <boost/type_index.hpp>

using boost::typeindex::type_id_with_cvr;

template<typename... T>
void print_types(std::string_view info)
{
    const auto add_sep = [](auto&& s){std::cout << s << ", ";};
    std::cout << info;
    ( ... , add_sep(type_id_with_cvr<T>().pretty_name()) );
    std::cout << "\n";
}


int main()
{
    std::cout << "case for const int arr[2]:\n";

    int arr[2] = {1,2};

    auto [a0, a1] = arr;    // copies arr, a0, a1 refer to the elements of the copy
    static_assert(std::is_same_v<decltype(a1), int>, "");
    print_types<decltype(a0)>("auto [a0,a1]:               ");

    const auto [ca0, ca1] = arr;
    print_types<decltype(ca0)>("const auto [ca0,ca1]:       ");
    static_assert(std::is_same_v<decltype(ca0), const int>, "");

    auto& [a_r0, a_r1] = arr;   // refers to the elements of arr
    print_types<decltype(a_r0)>("auto& [a_r0,a_r1]:          ");
    static_assert(std::is_same_v<decltype(a_r0), int>, "");

    auto const& [a_cr0, a_cr1] = arr;
    print_types<decltype(a_cr0)>("const auto& [a_cr0,a_cr1]:  ");
    static_assert(std::is_same_v<decltype(a_cr0), const int>, "");

    auto&& [a_rr0, a_rr1] = arr;
    print_types<decltype(a_rr0)>("auto&& [a_rr0,a_rr1]:       ");
    static_assert(std::is_same_v<decltype(a_rr0), int>, "");


    // All of the reported types are that of the original member data, possibly with additional
    // qualifiers. However the structured binding are references/aliases to the member data,
    // and not copies. Changing the bindings changes the corresponsing member data field.

    std::cout << "\ncase for pair<int, double>:\n";
    std::pair<int, double> p{42, 3.14};

    auto [p0, p1] = p;
    static_assert(std::is_same_v<decltype(p0), int>, "");
    print_types<decltype(p0), decltype(p1)>("auto [p0, p1]:       ");

    const auto [cp0, cp1] = p;
    static_assert(std::is_same_v<decltype(cp0), const int>, "");
    print_types<decltype(cp0), decltype(cp1)>("auto [cp0, cp1]:     ");

    auto& [p_r0, p_r1] = p;
    static_assert(std::is_same_v<decltype(p_r0), int>, "");
    print_types<decltype(p_r0), decltype(p_r1)>("auto [p_r0, p_r1]:   ");

    const auto& [p_cr0, p_cr1] = p;
    static_assert(std::is_same_v<decltype(p_cr0), const int>, "");
    print_types<decltype(p_cr0), decltype(p_cr1)>("auto [p_cr0, p_cr1]: ");

    auto&& [p_rr0, p_rr1] = p;
    static_assert(std::is_same_v<decltype(p_rr0), int>, "");
    print_types<decltype(p_rr0), decltype(p_rr1)>("auto [p_rr0, p_rr1]: ");


    std::cout << "\ncase for tuple<int, double>:\n";
    std::tuple<int, double> t{42, 3.14};

    auto [t0, t1] = t;
    static_assert(std::is_same_v<decltype(t0), int>, "");
    print_types<decltype(t0), decltype(t1)>("auto [t0, t1]:       ");

    const auto [ct0, ct1] = t;
    static_assert(std::is_same_v<decltype(ct0), const int>, "");
    print_types<decltype(ct0), decltype(ct1)>("auto [ct0, ct1]:     ");

    auto& [t_r0, t_r1] = t;
    static_assert(std::is_same_v<decltype(t_r0), int>, "");
    print_types<decltype(t_r0), decltype(t_r1)>("auto [t_r0, t_r1]:   ");

    const auto& [t_cr0, t_cr1] = p;
    static_assert(std::is_same_v<decltype(t_cr0), const int>, "");
    print_types<decltype(t_cr0), decltype(t_cr1)>("auto [t_cr0, t_cr1]: ");

    auto&& [t_rr0, t_rr1] = p;
    static_assert(std::is_same_v<decltype(t_rr0), int>, "");
    print_types<decltype(t_rr0), decltype(t_rr1)>("auto [t_rr0, t_rr1]: ");
}
