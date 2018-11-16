#include <iostream>
#include <string>
#include <stdlib.h>


/**
 * Singleton should be considered only if all three of the following criteria are satisfied:
 * - Ownership of the single instance cannot be reasonably assigned
 * - Lazy initialization is desirable
 * - Global access is not otherwise provided for
 * Make the class of the single instance object responsible for creation, initialization, access,
 * and enforcement. Declare the instance as a private static data member.
 * Provide a public static member function that encapsulates all initialization code,
 * and provides access to the instance.
 */


class Singleton
{
public:
    static inline Singleton& get_instance() noexcept;

    Singleton(Singleton const&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton const&) = delete;
    Singleton& operator=(Singleton&&) = delete;

protected:
    ~Singleton() noexcept { std::cerr << "dtor\n"; }

private:
    Singleton() { std::cerr << "ctor\n"; }
};

inline Singleton& Singleton::get_instance() noexcept
{
    static Singleton instance{};
    return instance;
}


int main()
{
    Singleton& s1 = Singleton::get_instance();
    auto&& s2 = Singleton::get_instance();
}
