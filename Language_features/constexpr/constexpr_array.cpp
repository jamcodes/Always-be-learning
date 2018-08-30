#include <iostream>
#include <iterator>
#include <type_traits>
#include <utility>


template<typename T, std::size_t N>
class array_result
{
public:
    using value_type = T;
    using reference = std::add_lvalue_reference_t<T>;
    using const_reference = std::add_const_t<reference>; 
    using iterator = T*;
    using const_iterator = const T*;

    constexpr array_result() noexcept = default;
    // template<T... Ts>
    // constexpr array_result(std::integer_sequence<T,Ts...>) noexcept
    //     : data_{Ts...} { }

    constexpr auto size() const noexcept { return N; }

    constexpr auto& operator[](std::size_t n)
    {
        if (N <= n) throw std::out_of_range("array_result: index out of range");
        return data_[n];
    }

    constexpr const auto& operator[](std::size_t n) const
    {
        if (N <= n) throw std::out_of_range("array_result: index_out_of_range");
        return data_[n];
    }

    constexpr iterator begin() noexcept { return &data_[0]; }
    constexpr iterator end() noexcept { return &data_[N]; }
    constexpr const_iterator cbegin() const { return &data_[0]; }
    constexpr const_iterator cend() const { return &data_[N]; }
    constexpr const_iterator begin() const { return cbegin(); }
    constexpr const_iterator end() const { return cend(); }

private:
    // constexpr static inline std::size_t size_{N};
    T data_[N]{};   // necessary to default initialize
};

// template<typename T, T... Ts>
// array_result(std::integer_sequence<T,Ts...>) -> array_result<T, sizeof...(Ts)>;

// template<typename T, T... Ts>
// constexpr auto make_array(std::integer_sequence<T,Ts...> elems) noexcept -> array_result<T,sizeof...(Ts)>
// {
//     return array_result<T,sizeof...(Ts)>{elems};
// }

template<typename T, std::size_t N>
constexpr auto generate_array() -> array_result<T,N>
{
    auto res = array_result<T,N>{};
    auto i = 0.1;
    for(auto it = res.begin(); it != res.end(); ++it)
    {
        *it = i++;
    }
    return res;
}



int main()
{
    // constexpr auto a1 = array_result{std::make_integer_sequence<int, 42>()};
    constexpr auto a1 = generate_array<double, 42>();
    constexpr auto a3 = array_result<double, 11>{};
    for(auto it = std::begin(a1); it != std::end(a1); ++it){
        std::cout << *it << ", ";
    }
}
