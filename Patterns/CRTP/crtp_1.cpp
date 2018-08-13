/**
 * An example of applying CRTP to create a base class used to count the number
 * of instances of a given class. This lets us avoid adding the same repetetive
 * code over and over to all the classes we intend to keep track of.
 */
#include <iostream>
#include <string>
#include <gsl/gsl>

using namespace std::string_literals;


template<typename CountedType>
class ObjectCounter
{
public:
    // return number of existing objects:
    static size_t live()
        {
            return ObjectCounter<CountedType>::count;
        }
protected:
    // default ctor
    ObjectCounter()
        {
            ++ObjectCounter<CountedType>::count;
        }
    // copy ctor
    ObjectCounter( const ObjectCounter<CountedType>& )
        {
            ++ObjectCounter<CountedType>::count;
        }
    // dtor
    ~ObjectCounter()
        {
            --ObjectCounter<CountedType>::count;
        }
private:
    static size_t count;    // number of existing objects
};
// initialize counter with zero
template<typename CountedType>
size_t ObjectCounter<CountedType>::count{0};


template<typename T>
class MyClass : public ObjectCounter<MyClass<T>>
{
public:
    T member{};
    MyClass( const T& t )
        : member{t} { }
    friend std::ostream& operator<<( std::ostream& os, const MyClass& obj )
    {
        return os << obj.member;
    }
};



int main()
{
    auto cs1 = MyClass<const char*>("cstring1");
    auto cs2 = MyClass<const char*>("cstring2");
    auto s1 = MyClass<std::string>("std::string1");
    auto czs = MyClass<typename gsl::czstring<>>("gsl::czstring1");

    std::cout << "Number of MyClass<const char*>: " << MyClass<const char*>::live()
              << std::endl;
    std::cout << "Number of MyClass<std::string>: " << s1.live() << std::endl;
    std::cout << "Number of MyClass<gsl::czstring<>>: " << czs.live() << std::endl;
}
