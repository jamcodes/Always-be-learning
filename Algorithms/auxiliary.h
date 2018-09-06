#pragma once

#include <chrono>
#include <random>
#include <string_view>
#include <algorithm>

template<typename T>
class rand_gen
{
public:
    using value_type = std::decay_t<T>;
    rand_gen(T low, T high)
        : ud_{low, high} { }

    auto operator()() const noexcept { return ud_(re_); }
    operator value_type() const noexcept { return operator()(); }
private:
    static inline std::default_random_engine re_{
        static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count())
    };

    mutable std::uniform_int_distribution<T> ud_;
};

template<typename Coll>
void print_elements(const Coll& coll, std::string_view str="")
{
    std::cout << str;
    for(const auto& el : coll){
        std::cout << el << ", ";
    }
    std::cout << "\n";
}
