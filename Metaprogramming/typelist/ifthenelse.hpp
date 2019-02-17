#pragma once

template<bool B, typename T, typename U>
struct if_then_else { using type = T; };

template<typename T, typename U>
struct if_then_else<false,T,U> { using type = U; };

template<bool B, typename T, typename U>
using if_then_else_t = typename if_then_else<B,T,U>::type;
