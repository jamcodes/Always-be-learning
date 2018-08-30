// In debug builds define
// #define GSL_THROW_ON_CONTRACT_VIOLATION
// In release builds define
// #define GSL_UNENFORCED_ON_CONTRACT_VIOLATION    // Segmentation fault on GCC
// or use assert() and define / undefine NDEBUG
#include <iostream>
#include <chrono>
#include <string>
#include <typeinfo>
#include <cassert>
#include <boost/type_index.hpp>
#if !defined(NDEBUG)
#define GSL_THROW_ON_CONTRACT_VIOLATION
#include <gsl/gsl>
#else
#define Expects(cond)
#define Ensures(cond)
#endif

using std::cout; using std::wcout; using std::endl;
using namespace std::string_literals;


// User-defined literals
class Probability
{
public:
    constexpr explicit Probability(long double v)
        : value(v)
        {
            Expects(v < 1.0 && 0.0 <= v);
            // assert(v < 1.0 && 0.0 <= v);
        }
private:
    long double value;

    friend constexpr Probability operator "" _prob(long double v);
    friend std::ostream& operator<<(std::ostream& os, const Probability& prob)
        { return os << "Probability{" << prob.value << "}"; }
};

class BadProbability : public std::logic_error
{ 
public:
    explicit BadProbability(long double v)
        : std::logic_error( "BadProbability exception, got value: " + std::to_string(v) )
        { }
};

constexpr Probability operator "" _prob(long double v)
{
    return 1.0 < v ? throw BadProbability{v} : Probability{v};
    // literals never represent negative values - no need to check for v < 0.0
}

using boost::typeindex::type_id_with_cvr;
int main()
try{
    // std::basic_string<wchar_t>
    auto s1 = "String literal"s;        // this is a std::string;
    auto s2 = L"Wide string literal"s;  // this is a std::wstring;
    auto s3 = std::basic_string<wchar_t>(L"This is a very explicit wstring");
    cout << "s1:\n\t" << s1 << "\n\ttypeid " << type_id_with_cvr<decltype(s1)>().pretty_name() << endl;
    wcout << L"s2:\n\t" << s2 << L"\n\ttypeid ";
    cout << type_id_with_cvr<decltype(s2)>().pretty_name() << endl;
    wcout << L"s3:\n\t" << s3 << L"\n\ttypeid ";
    cout << type_id_with_cvr<decltype(s3)>().pretty_name() << endl;

    cout << "\nUser defined literals\n" << "Probability\n" << endl;
    
    // constexpr auto prob1 = 1.2_prob;   // compiletime error;

    // auto prob2 = 1.2_prob;  // NOT a compiletimer error - throws at runtime

    constexpr auto prob3 = 0.3_prob;   // OK
        cout << prob3 << endl;

    // constexpr auto prob4 = Probability{1.2}; // Compiletime error

    constexpr auto prob5 = Probability{0.5};
        cout << prob5 << endl;

    auto prob6 = 0.6_prob;
        cout << prob6 << endl;

    // auto prob7 = Probability{1.7}; // runtime error
    //     cout << prob7 << endl;
}
catch( const std::exception& e )
{
    std::cerr << e.what () << endl;
}