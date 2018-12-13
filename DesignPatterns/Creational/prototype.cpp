#include <iostream>
#include <string>
#include <string_view>
#include <memory>


class Address {
public:
    Address(const std::string_view street, const std::string_view city, const int suite)
        : street_{street}, city_{city}, suite_{suite}
    {
    }

    Address(const Address&) = default;
    Address(Address&&) noexcept = default;
    Address& operator=(const Address&) = default;
    Address& operator=(Address&&) noexcept = default;
    ~Address() noexcept = default;

    Address* clone() const& { return new Address{*this}; }
    Address* clone() && { return new Address{std::move(*this)}; }

    friend std::ostream& operator<<(std::ostream& os, const Address& obj)
    {
        return os << "street: " << obj.street_
            << " city: " << obj.city_
            << " suite: " << obj.suite_;
    }

    friend class EmployeeFactory;
// private:     // normally we'd make this private and controll instantiation via friend Factory
    std::string street_{};
    std::string city_{};
    int suite_{};

};


class Contact {
public:
    Contact() = default;
    ~Contact() noexcept = default;
    Contact(const std::string_view name, Address const* const address)
        : name_{name}, address_{address->clone()}
    {
    }
    Contact(const std::string_view name, std::unique_ptr<Address> address)
        : name_{name}, address_{std::move(address)}
    {
    }
    Contact(const Contact& other)
        : name_{other.name_}, address_{other.address_->clone()}
    {
    }
    Contact(Contact&& other) noexcept
        : name_{std::move(other.name_)}, address_{std::move(other.address_)}
    {
    }
    Contact& operator=(const Contact& other)
    {
        name_ = other.name_;
        address_.reset(other.address_->clone());
        return *this;
    }
    Contact& operator=(Contact&& other)
    {
        name_ = std::move(other.name_);
        address_ = std::move(other.address_);
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Contact& c)
    {
        return os << "name: " << c.name_ << "\n\t address: " << *c.address_;
    }

    friend class EmployeeFactory;
// private:     // normally we'd make this private and constrol instantiation via friend Factory
    std::string name_{};
    std::unique_ptr<Address> address_{nullptr};
};


class EmployeeFactory {
public:
    static std::unique_ptr<Contact> NewMainOfficeEmployee(std::string_view name, int suite)
    {
        return NewEmployee(name, suite, main);
    }

    static std::unique_ptr<Contact> NewAuxOfficeEmployee(std::string_view name, int suite)
    {
        return NewEmployee(name, suite, aux);
    }

private:
    static inline const Contact main{"", std::make_unique<Address>("Main St.", "Home City", 0)};
    static inline const Contact aux{"", std::make_unique<Address>("Side St.", "Remote City", 0)};

    static std::unique_ptr<Contact> NewEmployee(const std::string_view name, int suite,
                                                 const Contact& proto)
    {
        auto result = std::make_unique<Contact>(proto);
        result->name_ = name;
        result->address_->suite_ = suite;
        return result;
    }
};

int main()
{
    // constructing Contacts manually is tedious
    // Contact john{"John Doe", std::make_unique<Address>("123 East Dr", "London", 123)};
    // Contact jane{"Jane Doe", std::make_unique<Address>("123 East Dr", "London", 124)};

    // construct the common part and copy (or clone) it as necessary
    const auto addr = std::make_unique<Address>("123 East Dr", "London", 0);

    Contact john{"John Doe", addr.get()};
    john.address_->suite_ = 123;

    Contact jane{"Jane Doe", addr.get()};
    jane.address_->suite_ = 124;

    std::cout << john << "\n" << jane << "\n";
    // We could also define a prototype Contact
    Contact employee{"Unknown", std::make_unique<Address>("628 Happy St", "Joy", 0)};
    auto john2{employee};
    john2.name_ = "John Doe";
    john2.address_->suite_ = 123;
    auto jane2{employee};
    jane2.name_ = "Jane Doe";
    jane2.address_->suite_ = 125;

    std::cout << john2 << "\n" << jane2 << "\n";
    // Better yet - control instantiation and make sure complete construction and initialization
    // is performed via a factory
    auto john3 = EmployeeFactory::NewAuxOfficeEmployee("John Doe", 123);
    auto jane3 = EmployeeFactory::NewMainOfficeEmployee("Jane Doe", 126);
    std::cout << *john3 << "\n" << *jane3 << "\n";
}
