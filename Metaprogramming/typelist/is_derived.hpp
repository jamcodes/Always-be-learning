#pragma once

#include "typelist.hpp"
#include "is_convertible.hpp"
#include "ifthenelse.hpp"
#include "is_same.hpp"
#include <type_traits>


template<typename Derived, typename Base>
struct is_derived_from
    : if_then_else_t<IsConvertible<Derived const*, Base const*> &&
                     !is_same_v<Derived const*, void const*>,
                     std::true_type,
                     std::false_type>
{
};

template<typename Base, typename Derived>
constexpr inline auto is_derived_from_v{is_derived_from<Base, Derived>::value};

namespace is_derived_from_unit_test
{
    struct B { };
    struct D : B { };
    struct A { };
    static_assert(is_derived_from_v<D, B>);
    static_assert(is_derived_from_v<D, A> == false);
    static_assert(is_derived_from_v<B, D> == false);
} // is_derived_unit_test
