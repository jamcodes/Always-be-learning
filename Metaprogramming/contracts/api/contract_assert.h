#pragma once

#include <utility>

#define CONTRACT_STRINGIFY_IMPL(x) #x
#define CONTRACT_STRINGIFY(x) CONTRACT_STRINGIFY_IMPL(x)

#if defined(__GNUC__) || defined(__clang__)
// Use `!!(cond)` to allow implicit conversions to bool?
// #define CONTRACT_LIKELY(x) __builtin_expect(!!(x), 1)
// #define CONTRACT_UNLIKELY(x) __builtin_expect(!!(x), 0)
#define CONTRACT_LIKELY(x) __builtin_expect((x), 1)
#define CONTRACT_UNLIKELY(x) __builtin_expect((x), 0)
#else
// #define CONTRACT_LIKELY(x) (!!(x))
// #define CONTRACT_UNLIKELY(x) (!!(x))
#define CONTRACT_LIKELY_(x) (x)
#define CONTRACT_UNLIKELY_(x) (x)
#endif

// From Microsoft's GSL
// CONTRACT_ASSUME(cond)
//
// Tell the optimizer that the predicate cond must hold. It is unspecified
// whether or not cond is actually evaluated.
//
#if defined(_MSC_VER)
#define CONTRACT_ASSUME(cond) __assume(cond)
#define CONTRACT_MARK_UNREACHABLE() __assume(0)
#elif defined(__GNUC__)
#define CONTRACT_ASSUME(cond) ((cond) ? static_cast<void>(0) : __builtin_unreachable())
#define CONTRACT_MARK_UNREACHABLE() __builtin_unreachable()
#elif defined(__has_builtin)
#if __has_builtin(__builtin_unreachable)
#define CONTRACT_ASSUME(cond) ((cond) ? static_cast<void>(0) : __builtin_unreachable())
#define CONTRACT_MARK_UNREACHABLE() __builtin_unreachable()
#endif
#else
#define CONTRACT_ASSUME(cond) static_cast<void>((cond) ? 0 : 0)
#define CONTRACT_MARK_UNREACHABLE()
#endif

#if !defined(CONTRACT_FORCE_INLINE)
#if defined(__GNUC__)
#define CONTRACT_FORCE_INLINE [[gnu::always_inline]] inline
#elif defined(_MSC_VER)
#define CONTRACT_FORCE_INLINE __forceinline
#elif defined(__has_attribute)
#if __has_attribute(always_inline)
#define CONTRACT_FORCE_INLINE __attribute__((always_inline))
#endif
#else
#define CONTRACT_FORCE_INLINE inline
#endif
#endif

#if !defined(CONTRACT_NEVER_INLINE)
#if defined(_MSC_VER)
#define CONTRACT_NEVER_INLINE __declspec(noinline)
#elif defined(__has_attribute)
#if __has_attribute(noinline)
#define CONTRACT_NEVER_INLINE __attribute__((noinline))
#endif  // __has_attribute(noinline)
#else
#define CONTRACT_NEVER_INLINE
#endif  // __has_attribute
#endif  // CONTRACT_NEVER_INLINE

namespace jam {

struct source_location final {
    const char* file;
    unsigned line;
};

#define CONTRACT_SRC_LOC() \
    jam::source_location { __FILE__, static_cast<unsigned>(__LINE__) }

namespace detail {
template <bool B, typename True, typename False>
struct IfThenElse {
    using type = False;
};
template <typename True, typename False>
struct IfThenElse<true, True, False> {
    using type = True;
};
}  // namespace detail

template <unsigned Level>
struct ContractLevel {
    using type = ContractLevel;
    using value_type = unsigned;
    static constexpr unsigned level{Level};
    constexpr operator value_type() const noexcept { return level; }
};

using Assume = ContractLevel<0>;
using Ignore = ContractLevel<1>;
using Enforce = ContractLevel<2>;

template <typename Handler, unsigned Level>
struct AssertionLevelIgnoreAssume
    : detail::IfThenElse<Handler::level == Assume::level &&
                             (Level == Enforce::level || Level == Assume::level),
                         Assume, Ignore> {
};

template <typename Handler, unsigned Level>
struct AssertionLevelEnforce
    : detail::IfThenElse<Level <= Handler::level && (Level > Ignore::level), Enforce,
                         typename AssertionLevelIgnoreAssume<Handler, Level>::type> {
};

template <typename Handler, typename Level>
using AssertionLevel = typename AssertionLevelEnforce<Handler, Level::level>::type;

struct noop_handler {
    template <typename... Args>
    static void handle(source_location const&, const char* const, Args&&...) noexcept
    {
    }
};

// Consider forcing this noinline to potentially improve codegen?
template <class Handler, typename... Args>
constexpr void contract_assert_failed(
    source_location const& loc, const char* const expr_str,
    Args&&... args) noexcept(noexcept(Handler::handle(loc, expr_str, std::forward<Args>(args)...)))
{
    return Handler::handle(loc, expr_str, std::forward<Args>(args)...), static_cast<void>(0);
}

// Consider fodcing these inline to potentially improve codegen?
template <typename Handler, typename Level = Enforce, typename = AssertionLevel<Handler, Level>>
struct ContractAssert final {
    template <typename Expr, typename... Args>
    static constexpr inline void do_assert(
        Expr const& expr, source_location const& loc, const char* const expr_str,
        Args&&... args) noexcept(noexcept(Handler::handle(loc, expr_str,
                                                          std::forward<Args>(args)...)))
    {
        return CONTRACT_LIKELY(expr())
                   ? static_cast<void>(0)
                   : contract_assert_failed<Handler>(loc, expr_str, std::forward<Args>(args)...);
    }
};

template <typename Handler, typename Level>
struct ContractAssert<Handler, Level, Ignore> final {
    template <typename Expr, typename... Args>
    static constexpr inline void do_assert(Expr const&, source_location const&, const char* const,
                                           Args&&...) noexcept
    {
    }
};

template <typename Handler, typename Level>
struct ContractAssert<Handler, Level, Assume> final {
    template <typename Expr, typename... Args>
    static constexpr inline void do_assert(Expr const& expr, source_location const&,
                                           const char* const, Args&&...) noexcept
    {
        return CONTRACT_ASSUME(expr());
    }
};

template <typename Expr, typename Handler, unsigned Level, typename... Args>
constexpr inline void do_contract_assertion(
    Expr const& expr, source_location const& loc, const char* const expr_str, Handler const,
    ContractLevel<Level> const,
    Args&&... args) noexcept(noexcept(ContractAssert<Handler, ContractLevel<Level>>::
                                          do_assert(expr, loc, expr_str,
                                                    std::forward<Args>(args)...)))
{
    return ContractAssert<Handler, ContractLevel<Level>>::do_assert(expr, loc, expr_str,
                                                                    std::forward<Args>(args)...);
}

template <typename Expr, typename Handler, typename... Args>
constexpr inline void do_contract_assertion(
    Expr const& expr, source_location const& loc, const char* const expr_str, Handler const,
    Args&&... args) noexcept(noexcept(ContractAssert<Handler,
                                                     Enforce>::do_assert(expr, loc, expr_str,
                                                                         std::forward<Args>(
                                                                             args)...)))
{
    return ContractAssert<Handler, Enforce>::do_assert(expr, loc, expr_str,
                                                       std::forward<Args>(args)...);
}

}  // namespace jam

// TODO: Decide which frontend interface to choose

#define CONTRACT_ASSERT(eXPR, hANDLER, lEVEL, ...)                                      \
    static_cast<void>(                                                                  \
        jam::ContractAssert<hANDLER, lEVEL>::do_assert([&]() noexcept { return eXPR; }, \
                                                       CONTRACT_SRC_LOC(), #eXPR, __VA_ARGS__))

#define CONTRACT_EXPECTS(eXPR, hANDLER, lEVEL, ...)                                      \
    static_cast<void>(                                                                  \
        jam::ContractAssert<hANDLER, lEVEL>::do_assert([&]() noexcept { return eXPR; }, \
                                                       CONTRACT_SRC_LOC(), #eXPR, __VA_ARGS__))

#define CONTRACT_ENSURES(eXPR, hANDLER, lEVEL, ...)                                      \
    static_cast<void>(                                                                  \
        jam::ContractAssert<hANDLER, lEVEL>::do_assert([&]() noexcept { return eXPR; }, \
                                                       CONTRACT_SRC_LOC(), #eXPR, __VA_ARGS__))

#define CONTRACT_ASSERTION(eXPR, ...)                                             \
    static_cast<void>(jam::do_contract_assertion([&]() noexcept { return eXPR; }, \
                                                 CONTRACT_SRC_LOC(), #eXPR, __VA_ARGS__))
