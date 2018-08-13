#include <iostream>
#include <string>
#include <type_traits>


// 1 --- unconstraint template
/* ------------------------------------------------------------------------- */
// template<typename T, typename U>
// bool check(T&& lhs, U&& rhs)
// {
//     return lhs == rhs;
// }
/* ------------------------------------------------------------------------- */


// 2 --- enable_if / SFINAE
/* ------------------------------------------------------------------------- */
// template<typename T, typename U, typename = void>
// struct is_equality_comparable : std::false_type { };

// template<typename T, typename U>
// struct is_equality_comparable<T, U,
//     std::enable_if_t<
//         true,
//         decltype(std::declval<T&>() == std::declval<U&>(), (void)0)
//         >
//     > : std::true_type
// {
// };

// template<typename T, typename U, 
//          typename = std::enable_if_t<is_equality_comparable<T,U>::value>
//         >
// bool check(T&& lhs, U&& rhs) { return lhs == rhs; }
/* ------------------------------------------------------------------------- */


// Enter Concepts!
/* ------------------------------------------------------------------------- */

// 1 --- Most basic use
/* ------------------------------------------------------------------------- */
// template<typename T, typename U>
//     requires requires(T t, U u) { t == u; }
// bool check(T&& lhs, U&& rhs) { return lhs == rhs; }
/* ------------------------------------------------------------------------- */

// 2.1 --- Define a named concept
/* ------------------------------------------------------------------------- */
// template<typename T, typename U>
// concept bool Equality_comparable() {
//     return requires(T t, U u){
//         { t == u } -> bool;
//     };
// }

// template<typename T, typename U>
//     requires Equality_comparable<T,U>()
// bool check(T&& lhs, U&& rhs) {return lhs == rhs;}
/* ------------------------------------------------------------------------- */

// 2.2 --- Alternative syntax
/* ------------------------------------------------------------------------- */
template<typename T, typename U>
concept bool Equality_comparable =
    requires(T t, U u){
        { t == u } -> bool;
    };

template<typename T, typename U>
    requires Equality_comparable<T,U>   // Notice no function call operator here
bool check(T&& lhs, U&& rhs) {return lhs == rhs;}
/* ------------------------------------------------------------------------- */

// 2.3 --- Make concepts consistent, symmetric and general
/* ------------------------------------------------------------------------- */
template<typename T, typename U>
concept bool EqualityComparable =
    requires(T t, U u){
        { t == u } -> bool;
        { u == t } -> bool;
        { t != u } -> bool;
        { u != t } -> bool;
    };

template<typename T, typename U>
    requires EqualityComparable<T,U>
bool equality_check(T&& lhs, U&& rhs) { return lhs == rhs; }

// --- this imposes more requirements on the users of the template, now both
// (==) and (!=) operators must be declared (not necessarily defined), which
// actually means all four operators must be declared.
// Arguably in this example this "Over-constraints" our template,
// we don't actually use the operators but they're still required.

struct Fails_EQ {
    int i;
    
    bool operator==(const Fails_EQ& other) const { return i == other.i; }
    bool operator==(int other) const { return i == other; }
};

struct Satisfies_EQ {
    int i;
    
    bool operator==(const Satisfies_EQ& other)const { return i == other.i; }
    bool operator!=(const Satisfies_EQ& other)const { return i != other.i; }
};

bool operator==(const Satisfies_EQ& lhs, int rhs) { return lhs.i == rhs; }
bool operator!=(const Satisfies_EQ& lhs, int rhs) { return lhs.i != rhs; }
bool operator==(int lhs, const Satisfies_EQ& rhs) { return lhs == rhs.i; }
bool operator!=(int lhs, const Satisfies_EQ& rhs) { return lhs != rhs.i; }
/* ------------------------------------------------------------------------- */

// 2.4 --- Concept use for more complex cases - ctor constraints
/* ------------------------------------------------------------------------- */
struct MyStruct {
    enum Type_t {int_t, float_t};
    Type_t m_type;

// Construct from 'Int' values
    template<typename T>
        requires std::is_integral<T>::value
    MyStruct(T&&) : m_type{int_t} { }

// Construct from 'Float" values
    template<typename T>
        requires std::is_floating_point<T>::value
    MyStruct(T&&) : m_type{float_t} { }
};

std::ostream& operator<<(std::ostream& os, const MyStruct& ms)
{ return os << ((ms.m_type == MyStruct::int_t) ? "int_t" : "float_t"); }
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */

int main()
{

    std::cout << "valid template use - check (1, 1) == " << check(1, 1) << std::endl;

// --- Compile time error - constraints not satisfied
    std::cout << "Invalid template use - check(1, std::string(\"Concepts\")) == "
        << check(1, std::string("Concepts")) << std::endl;


// --- 2.3
    std::cout << "Satisfied constraints: "
        << equality_check(Satisfies_EQ{42}, 42) << std::endl;
    std::cout << "Unsatisfied constraints: "
        << equality_check(Fails_EQ{42}, 42) << std::endl;

// --- 2.4
    const auto ms_int = MyStruct{42};
    std::cout << "MyStruct{42} -> " << ms_int << std::endl;
    const auto ms_float = MyStruct{3.1415}; // implicit conversion aparently
    std::cout << "MyStruct{3.1415} -> " << ms_float << std::endl;
// still implicit conversion. Would probably fail due to narrowing
// conversion if I actually tried assigning it to a float type var.
    const auto ms_double = MyStruct{2.71828182845904523536};
    std::cout << "My_struct{e} -> " << ms_double << std::endl;
    
// --- Compile time error - constructor constraints not satisfied
    const auto ms_invalid = MyStruct{"Concepts"};
    std::cout << "MyStruct{\"Concepts\"}" << ms_invalid << std::endl;

    return 0;
}
