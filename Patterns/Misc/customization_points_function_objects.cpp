#include <iostream>
#include <type_traits>

// Traditionally using ADL for customization points requires a `using ...` two-step, e.g.:
// generic function not knowing if the type it operates on implements it's own swap() overload...
template<typename T>
void foo_swap_two_step(T& obj)
{
    T tmp{};
    // bring std::swap into scope, in case type T doesn't implement it's own swap(T&, T&) overload
    using std::swap;
    swap(obj, tmp);
    // ... more code
}
// Failing to perform the two-step and calling std::swap() directly is a potential inefficiency
// or even a bug. It needs to be rememberd and applied every time a free function is used
// as a customization point in combination with ADL.

// Problem - it's easily forgotten, verbose and needs to be mindlessly repeated

// Solution - provide a global function object which call operator applies the appropriate two-step
namespace std
{
  namespace __detail
  {
    // define begin for arrays
    template<class T, size_t N>
    constexpr T* begin(T (&a)[N]) noexcept
    {
      return a;
    }

    // Define begin for containers
    // (trailing return type needed for SFINAE)
    template<class _RangeLike>
    constexpr auto begin(_RangeLike && rng) ->
      decltype(forward<_RangeLike>(rng).begin())
    {
      return forward<_RangeLike>(rng).begin();
    }

    struct __begin_fn
    {
      template<class R>
      constexpr auto operator()(R && rng) const ->
        decltype(begin(forward<R>(rng)))
      {
        return begin(forward<R>(rng));
      }
    };
  }

  // To avoid ODR violations:
  template<class T>
  struct __static_const
  {
    static constexpr T value{};
  };

  template<class T>
  constexpr T __static_const<T>::value;

  // std::begin is a global function object!
  namespace
  {
    constexpr auto const& begin =
        __static_const<__detail::__begin_fn>::value;

    // In C++17 we could just define an inline global:
    inline constexpr auto const begin = __detail::__begin_fn{};
  }
}

// As an added benefit - the std::begin global function object can now be passed in
// as an argument to algorithms

int main()
{

}
