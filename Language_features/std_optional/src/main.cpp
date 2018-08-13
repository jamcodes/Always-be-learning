#include <optional>
#include <utility>
#include <iostream>
#include <random>
#include <chrono>

using namespace std::string_literals;

namespace
{
std::uniform_int_distribution<> ud;
std::default_random_engine re{
    static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count())};
} // namespace


class locked_device
{
    explicit locked_device(const char* /*param*/)
    {
        std::cout << "Device is locked\n";
    }
public:
    void use() const noexcept
    {
        std::cout << "Success!\n";
    }

    static std::optional<locked_device> try_lock_device()
    {
        if (ud(re) % 2) {
            return std::nullopt;
        }
        return locked_device("Device name");
    }
};

int main()
{
    for(int i = 0; i < 10; ++i) {
        const auto dev = locked_device::try_lock_device();
        if (dev) {
            dev->use();
            break;
        }
        std::cout << "... trying again\n";
    }

// semantics of the type

    // same as std::optional{std::nullopt};
    auto val = std::optional<std::string>{};    // it is default constructible

    // default constructed std::optional doesn't hold a value
    if (!val) { // is convertible to bool
        std::cout << "\nval doesn't hold a value\n";
    }

    // constructing std::optional also costs us a move construction of the contained variable
    auto val2 = std::optional<std::string>{"std::string connstructed, and moved"};

    // Move costs can be avoided by constructing in-place
    // This is also necessary if a type can not be copied or moved
    auto val3 = std::optional<std::string>{std::in_place, "Constructed in_place"};

    // or by using std::make_optional - which essentially constructs in place
    auto val4 = std::make_optional<std::string>("make_optioinal");

    // assigning a values either move, or copy assigns it
    val = "Hello!"; // constructs a std::string from const char* and moves it into std::optional

    // the move can be avoided by constructing in place - using emplace()
    val.emplace("Emplaced");

    // dereferencing std::optional returns a reference to the contained object
    std::cout << "val2 = " << *val2 << "\n";

    // -> operator returns a pointer to the contained object
    std::cout << "val2->c_str() = " << val2->c_str() << "\n";

    // both operator*() and operator->() are unchecked
    // behavior is undefined if the object does not contain a value
    // It can be checked if std::optional contains a value by a conversion to bool
    if (val2) {
        std::cout << "val2 bool conversion = " << *val2 << "\n";
    }
    // or explicitly
    if (val2.has_value()) {
        std::cout << "val2.has_value() = " << *val2 << "\n";
    }

    // If a checked access is needed std::optional provides two alternatives:
    // .value() - throws if doesn't contain a value
    std::cout << "val3.value() = " << val3.value() << "\n";
    // .value_or() - returns the contained object's value, or the given alternative - noexcept
    std::cout << "val3.value_or(\"some default\") = " << val3.value_or("some default"s) << "\n";

    // The contained object can be destroyed by using .reset()
    val3.reset();
    std::cout << "val3.value_or(\"some default\") after .reset() = "
        << val3.value_or("some default"s) << "\n";
    // or by assigning std::nullopt
    val4 = std::nullopt;
    try {
        std::cout << val4.value() << "\n";
    }
    catch(const std::bad_optional_access& e) {
        std::cout << "val4.value() threw an exception\n";
    }
}
