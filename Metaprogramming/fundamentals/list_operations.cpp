#include <iostream>
#include <type_traits>


template<typename... Ts>
struct list { };

// list of values
template<typename T, T... Values>
using integral_list = list<std::integral_constant<T,Values>...>;

// last element of a list
template<typename...> struct back;

template<typename T> struct back<T> { using type = T; };
template<typename T> struct back<list<T>> { using type = T; };

template<typename T, typename... Ts>
struct back<T, Ts...>
{
    using type = typename back<Ts...>::type;
};

template<typename T, typename... Ts>
struct back<list<T,Ts...>>
{
    using type = typename back<Ts...>::type;
};

template<typename T> using back_t = typename back<T>::type;

// first element
template<typename...> struct front;

template<typename Head, typename... Tail>
    struct front<Head, Tail...> { using type = Head; };

template<typename Head, typename... Tail>
    struct front<list<Head,Tail...>> { using type = Head; };

template<typename Head, typename... Tail>
    using front_t = typename front<Head,Tail...>::type;


int main()
{
    using mylist = list<int, char, double>;
    static_assert(sizeof(typename back<mylist>::type) == sizeof(double), "back_t didn't return double");
    static_assert(sizeof(typename front<mylist>::type) == sizeof(int), "front_t didn't return int");
}
