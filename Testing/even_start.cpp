#include <iostream>
#include <thread>
#include <future>
#include <string>
#include <chrono>
#include <tuple>
#include <utility>
#include <type_traits>
#include <ctime>
#include <iomanip>
#include "even_start.hpp"

template<typename Tuple, std::size_t... Is>
constexpr void get_all_impl(Tuple&& t, std::index_sequence<Is...>)
{
    const auto print{[](auto tp){
            auto const time{std::chrono::system_clock::to_time_t(tp)};
            std::cout << std::put_time(std::localtime(&time), "%c") << "\n";
        }};
    ( ... , print(std::get<Is>(std::forward<Tuple>(t)).get()) );
}

template<typename Tuple>
constexpr void get_all(Tuple&& t)
{
    using Indices = std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>;
    get_all_impl(std::forward<Tuple>(t), Indices{});
}

int main()
{
    using namespace std::chrono_literals;
    auto const call{ [](){return std::chrono::system_clock::now();} };
    auto async_results{ even_start(call, call, call, call) };
    std::cout << "Time point before start = ";
    auto const t{std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())};
    std::cout << std::put_time(std::localtime(&t), "%c") << "\n";
    get_all(async_results);
}
