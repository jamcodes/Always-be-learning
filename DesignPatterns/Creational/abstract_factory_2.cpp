#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>


struct HotDrink {
public:
    virtual ~HotDrink() = default;
    virtual void prepare(int volume) = 0;
};

struct Tea : public HotDrink {
public:
    void prepare(int volume) override { std::cout << "Prepare " << volume << "ml of tea\n"; }
};

struct Coffee : public HotDrink {
public:
    void prepare(int volume) override
    {
        std::cout << "Prepare " << volume << "ml of delicious coffee\n";
    }
};

// --- Abstract factory for HotDrinks
struct HotDrinkFactory {
public:
    virtual ~HotDrinkFactory() = default;
    virtual std::unique_ptr<HotDrink> make() const = 0;
};

struct TeaFactory : public HotDrinkFactory {
public:
    std::unique_ptr<HotDrink> make() const override { return std::make_unique<Tea>(); }
};

struct CoffeeFactory : public HotDrinkFactory {
public:
    std::unique_ptr<HotDrink> make() const override { return std::make_unique<Coffee>(); }
};

class DrinkFactory {
public:
    DrinkFactory()
    {
        factories_["coffee"] = std::make_unique<CoffeeFactory>();
        factories_["tea"] = std::make_unique<TeaFactory>();
    }

    std::unique_ptr<HotDrink> make_drink(const std::string& name)
    {
        auto drink = factories_[name]->make();
        drink->prepare(200);
        return drink;
    }
private:
    std::unordered_map<std::string, std::unique_ptr<HotDrinkFactory>> factories_{};
};


int main()
{
    DrinkFactory df{};
    df.make_drink("coffee");
    df.make_drink("tea");
}
