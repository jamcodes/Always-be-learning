#include <iostream>
#include <memory>
#include <string>
#include <utility>


class PersonBuilder;
class PersonAddressBuilder;
class PersonJobBuilder;

class Person {
public:
    friend class PersonBuilder;
    friend class PersonAddressBuilder;
    friend class PersonJobBuilder;

    ~Person() noexcept
    {
        std::cerr << "Person dtor\n";
    }

    Person(Person const&) = default;
    Person(Person&&) noexcept = default;
    Person& operator=(Person&&) noexcept = default;
    Person& operator=(Person const&) = default;

    static PersonBuilder create();

    friend std::ostream& operator<<(std::ostream& os, Person const& p)
    {
        return os
            << "street_address: " << p.street_address_
            << "\npost_code: " << p.post_code_
            << "\ncity: " << p.city_
            << "\ncompany_name: " << p.company_name_
            << "\nposition: " << p.position_
            << "\nannual_income: " << p.annual_income_;
    }

private:
    Person()
    {
        std::cerr << "Person ctor\n";
    }

    std::string street_address_{};
    std::string post_code_{};
    std::string city_{};
    std::string company_name_{};
    std::string position_{};
    int annual_income_{};
};

class PersonBuilderBase {
public:
    operator Person() const noexcept { return std::move(person_); }

    // builder facets
    PersonAddressBuilder lives() const;
    PersonJobBuilder works() const;
protected:
    explicit PersonBuilderBase(Person& person)
        : person_{person}
    {
    }

    Person& person_;
};

class PersonBuilder : public PersonBuilderBase {
public:
    PersonBuilder()
        : PersonBuilderBase{p_}
    {
    }

private:
    Person p_{};
};

class PersonAddressBuilder : public PersonBuilderBase {
    using self = PersonAddressBuilder;
public:
    explicit PersonAddressBuilder(Person& person)
        : PersonBuilderBase{person}
    {
    }

    self& at(std::string street_address)
    {
        person_.street_address_ = std::move(street_address);
        return *this;
    }

    self& with_postcode(std::string post_code)
    {
        person_.post_code_ = std::move(post_code);
        return *this;
    }

    self& in(std::string city)
    {
        person_.city_ = std::move(city);
        return *this;
    }
};

class PersonJobBuilder : public PersonBuilderBase {
    using self = PersonJobBuilder;
public:
    // using PersonBuilderBase::PersonBuilderBase;
    explicit PersonJobBuilder(Person& person)
        : PersonBuilderBase{person}
    {
    }

    self& at(std::string company_name)
    {
        person_.company_name_ = std::move(company_name);
        return *this;
    }

    self& as_a(std::string position)
    {
        person_.position_ = std::move(position);
        return *this;
    }

    self& earning(int annual_income)
    {
        person_.annual_income_ = annual_income;
        return *this;
    }
};


PersonBuilder Person::create()
{
    return PersonBuilder{};
}

PersonAddressBuilder PersonBuilderBase::lives() const
{
    return PersonAddressBuilder{person_};
}

PersonJobBuilder PersonBuilderBase::works() const
{
    return PersonJobBuilder{person_};
}


int main()
{
    Person p = Person::create()
        .lives().at("123 London Road").with_postcode("SW1 1GB").in("London")
        .works().at("PragmaSoft").as_a("Consultant").earning(10e6);

    std::cerr << p << "\n";
}
