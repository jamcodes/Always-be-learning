#include <iostream>
#include <type_traits>
#include <boost/type_index.hpp>

using boost::typeindex::type_id;


template<typename T1, typename T2>
void type_traits_example(T1& v1, T2& /*v2*/)
{
    std::cout << "\n" << __PRETTY_FUNCTION__ << "\n";
    std::cout << "is_unsigned_v<T1> = " << std::is_unsigned_v<T1> << "\n";
    std::cout << "is_same_v<T1,T2>  = " << std::is_same_v<T1,T2> << "\n";

    using t1_nonconst_t = std::remove_const_t<T1>;
    static_assert(!std::is_const_v<t1_nonconst_t>, "remove_const_t<T1> returned non-const T");
    std::cout << "remove_const_t<t1_nonconst_t> = "
        << type_id<t1_nonconst_t>().pretty_name().c_str() << "\n";

    using v1_nonconst_t = std::remove_const_t<decltype(v1)>;
    static_assert(!std::is_const_v<v1_nonconst_t>,
                  "remove_const_t<decltype(v1)> returned const T");
    std::cout << "decltype(v1) = "
        << type_id<decltype(v1)>().pretty_name().c_str() << "\n"
        << "remove_const_t<v1_nonconst_T> = "
        << type_id<v1_nonconst_t>().pretty_name().c_str() << "\n";
}


int main()
{
    const auto u = unsigned{42};
    auto       i = int{-42};
    type_traits_example(u, i);
    type_traits_example(i, u);
}
