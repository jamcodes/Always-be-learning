#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <type_traits>
#include <tuple>


// folds
/* --------------------------------------------------------------------------------------------- */
/**
 * Fold expression types:
 * left fold:       ( ... op pack )         -> (((el1 op el2) op el3) ... op elN)
 * right fold:      ( pack op ... )         -> (el1 op (... (elN-1 op elN)))
 * left fold init:  (init op ... op pack)   -> (((init op el1) op el2) ... op elN)
 * right fold init: (pack op ... op init)   -> (el1 op (... (elN op init)))
 */
template<typename... Ts>
constexpr auto foldSum(Ts&&... s) noexcept
{
    return (... + s);   // lfold ((s1 + s2) + s3) ...
}
/* --------------------------------------------------------------------------------------------- */


// printing all arguments - we need to wrap each arg, so that a separator is added
/* --------------------------------------------------------------------------------------------- */
template<typename T>
class AddSpace
{
public:
    constexpr AddSpace(const T& r) : ref_{r} { }
    friend std::ostream& operator<<(std::ostream& os, AddSpace s)
    { return os << s.ref_ << ' '; }
private:
    const T& ref_;
};

template<typename... Ts>
void print(const Ts&... args)
{
    ( std::cout << ... << AddSpace{args} ) << "\n";
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
template<typename... Ts>
void printDoubled(const Ts&... args)
{
    print(args + args...);
    // both pack instances are expanded... -> print(el1 + el1), print(el2 + el2) ...
}

template<typename... Ts>
void printAddOne(const Ts&... args)
{
    // print(args + 1...)   // ERROR: 1... is a literal with too many decimal points
    // print(args + 1 ...)  // OK
    print((args + 1)...);   // OK
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
template<typename C, typename... Idx>
void printElems(const C& coll, Idx... idx)
{
    print(coll[idx]...);
}

template<std::size_t... Idx, typename C>
void printIdx(const C& coll)
{
    print(coll[Idx]...);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// using index_sequence to access via std::get
template<typename T, std::size_t... Idx>
void printByIdx(T&& t, std::index_sequence<Idx...>)
{
    print(std::get<Idx>(std::forward<T>(t))...);
}

/* --------------------------------------------------------------------------------------------- */

// using variadic packs in argument deduction guides and type_traits...
/* --------------------------------------------------------------------------------------------- */
// std::array defines:
// template<typename T, typename... Ts>
// myarray(T, Ts...) -> myarray<std::enable_if<(std::is_same_v<T, Ts> && ...), T>,
//                              (1 + sizeof...(Ts))>
// This checkes if all types are the same and deduces the size based on the parameter pack
// sizeof... - note the +1 due to one element being slised off into T;
/* --------------------------------------------------------------------------------------------- */


int main()
{
    std::cout << "foldSum(1,2,3,4) = " << foldSum(1, 2, 3, 4) << "\n";
    std::cout << "print(42, 3.141518, \"Hello\", ',', std::string(\"World!\")): ";
    print(42, 3.141518, "Hello", ',', std::string{"World!"});
    std::cout << "printDoubled(std::string{\"Hello\"}, 42): ";
    
    printDoubled(std::string{"Hello"}, 42);
    std::cout << "printAddOne(1,2,3): ";
    
    printAddOne(1, 2, 3);
    const std::vector<std::string> coll = {"hello", "world", "cruel", "bye", "templates"};
    
    printElems(coll, 0u, 4u, 1u);
    printIdx<0u,4u,1u>(coll);

    std::array<std::string, 5> arr = {"Hello", "my", "new", "!", "World"};
    printByIdx(arr, std::index_sequence<0, 4, 3>{});
    const auto t = std::make_tuple(12, "monkeys", 2.1);
    printByIdx(t, std::index_sequence<0, 1, 2>{});
}
