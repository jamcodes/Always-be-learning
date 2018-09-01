#include <iostream>
#include <type_traits>
#include <utility>


// void_t - an alias template to check if an expression is well-formed
// Used in SFINAE context to remove ill-fromed template specialization from resolution
template<typename... T>
using void_t = void;


// Checking a condition based on if a type has a nested type
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
template<typename T, typename = void>
struct is_container : std::is_array<T> { };

// This specialization will be well-formed only if T has a nested type `iterator`
template<typename T>
struct is_container<T, void_t<typename T::iterator>> : std::true_type { };
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


// Checking a condition based on if a type has a member
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// declval - function used to convert a type to a reference witout the need to construct the type.
// Allows checking if a type has a member without constructing it
template<typename T>
std::add_rvalue_reference_t<T> mydeclval() noexcept;


template<typename T, typename U>
using my_test_impl = decltype(
    std::declval<T>().foo(std::declval<U>())    // any expression
);

template<typename T, typename U, typename = void>
struct my_thing : std::false_type { };

template<typename T, typename U>
struct my_thing<T, U, void_t<my_test_impl<T, U>>> : std::true_type { };

// could check any property of my_test_impl
// template<typename T, typename U>
// struct my_thing<T, U,
//      std::enable_if_t<std::is_same_v<my_test_impl<T,U>,int>>> : std::true_type { };
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


int main()
{

}
