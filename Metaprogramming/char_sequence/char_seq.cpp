#include <array>
#include <iostream>
#include <type_traits>
#include <utility>
#include <functional>

using std::size_t;

// as of C++17 std::array is constexpr consistent - can be used at compiletime
template<size_t... Is>
constexpr auto make_seq_array(std::index_sequence<Is...>)
{
    return std::array<size_t, sizeof...(Is)>{{Is...}};
}

template<size_t... Is>
constexpr auto make_seq_array(const size_t mul, std::index_sequence<Is...>)
{
    return std::array<size_t, sizeof...(Is)>{{mul*(1+Is)...}};
}

/**
 * 2D array of 'squares':
 *  1 2  3  4 ...
 *  2 4  6  8
 *  3 6  9 12
 *  4 8 12 16
 *  ...
 */
template<size_t... Is>
constexpr auto make_2d_seq_array(std::index_sequence<Is...>)
{
    return std::array< std::array<size_t,sizeof...(Is)>, sizeof...(Is)>
        {{make_seq_array(1+Is, std::make_index_sequence<sizeof...(Is)>{})...}};
}


template<char... S>
using char_sequence = std::integer_sequence<char, S...>;

template<char... S>
constexpr auto newline(char_sequence<S...>)
{
    return char_sequence<S..., '\n'>{};
}
constexpr auto newline()
{
    return char_sequence<'\n'>{};
}

constexpr auto
make_digit_char(const size_t digit, const size_t power_of_ten=1, const char fill=' ')
{
    // return static_cast<char>(digit >= power_of_ten ? digit/power_of_ten+'0' : fill);
    if (digit >= power_of_ten)
        return static_cast<char>(digit/power_of_ten + '0');
    return fill;
}

template<size_t N>
constexpr auto make_char_seq_from_num()
{
    static_assert(N < 1000, "Can not handle large numbers");
    return char_sequence<' ',
                make_digit_char(N, 100),
                make_digit_char(N%100, 10, N>=100?'0':' '),
                make_digit_char(N%10, 1, '0')
                // char(N%10+'0')
            >{};
}


/**
 * Concatenate two integer_sequences into one
 */
template<typename T, T... Xs, T... Ys>
constexpr auto concat_sequence(std::integer_sequence<T,Xs...>, std::integer_sequence<T,Ys...>)
{
    return std::integer_sequence<T,Xs...,Ys...>{};
}

template<size_t N, size_t... Is>
constexpr auto add(std::index_sequence<Is...>)
{
    return std::index_sequence<(N+Is)...>{};
}

template<size_t N, size_t... Is>
constexpr auto multiply(std::index_sequence<Is...>)
{
    return std::index_sequence<(N*Is)...>{};
}
// constexpr auto test_multiply = multiply<42>(std::make_index_sequence<10>());

template<typename Func, size_t...Is>
constexpr auto transform_sequence(std::index_sequence<Is...>, Func&& f)
{
    return std::index_sequence<f(Is)...>{};
}

constexpr auto test_transform = transform_sequence(std::make_index_sequence<10>(),
                                    [](size_t i){return i + 42;});

template<typename Func, typename T, T... Ts>
constexpr auto transform_seq(std::integer_sequence<T,Ts...>, Func&& f)
{
    using result_type = std::invoke_result_t<Func, T>;
    return std::integer_sequence<result_type, std::forward<Func>(f)(Ts)...>{};
}


constexpr auto test_transform_seq = transform_seq(std::make_index_sequence<10>(),
                                        [](auto i)->char{return static_cast<char>(i+'0');}
                                    );

template<size_t... Is>
struct convert_to_charseq_impl;

template<size_t I, size_t... Is>
constexpr auto convert_to_charseq()
{
    return concat_sequence(make_char_seq_from_num<I>(), convert_to_charseq_impl<Is...>{}());
}

template<size_t... Is>
struct convert_to_charseq_impl
{
    constexpr auto operator()() const
    {
        return convert_to_charseq<Is...>();
    }
};

template<>
struct convert_to_charseq_impl<>
{
    constexpr auto operator()() const
    {
        return char_sequence<>{};
    }
};


template<size_t... Is>
constexpr auto make_row_charseq(std::index_sequence<Is...>)
{
    return newline(convert_to_charseq<Is...>());
}

template<size_t Row, size_t N>
constexpr auto make_row()
{
    constexpr auto Indices = multiply<Row>(add<1>(std::make_index_sequence<N>{}));
    return make_row_charseq(Indices);
}

template<size_t n, size_t... cols>
struct make_rows_as_charseq_impl;

template<size_t n, size_t col, size_t... cols>
constexpr auto make_rows_as_charseq()
{
    return concat_sequence(make_row<col, n>(), make_rows_as_charseq_impl<n, cols...>{}());
}

template<size_t n, size_t... cols>
struct make_rows_as_charseq_impl
{
    constexpr auto operator()() const
    {
        return make_rows_as_charseq<n, cols...>();
    }
};

template<size_t n>
struct make_rows_as_charseq_impl<n>
{
    constexpr auto operator()() const
    {
        return char_sequence<>{};
    }
};

template<size_t n, size_t... cols>
constexpr auto makerows(std::index_sequence<cols...>)
{
    return make_rows_as_charseq<n,cols...>();
}

template<char... s>
constexpr const char make_char_string[] = {s..., '\n'};
template<char... s>
constexpr const auto make_char_string_from_sequence(char_sequence<s...>)
{
    return make_char_string<s...>;
}

template<size_t dim>
constexpr auto multiplication_table = make_char_string_from_sequence(concat_sequence(newline(),makerows<dim>(add<1>(std::make_index_sequence<dim>{}))));


constexpr const char* const result = multiplication_table<10>;

int main()
{
    std::cout << result;
}
