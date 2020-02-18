#ifndef TRACE_HEADER_GUARD_HPP_
#define TRACE_HEADER_GUARD_HPP_

#include <trace_level.h>

#if defined(ENABLE_TRACE_LOGS)

#include <iostream>
#include <utility>
namespace detail
{

template <typename charT, typename traits>
std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, TraceLevel level)
{
    switch (level)
    {
        case TraceLevel::Fatal:
            return os << "FATAL";
        case TraceLevel::Error:
            return os << "Error";
        case TraceLevel::Debug:
            return os << "Debug";
        case TraceLevel::Verbose:
            return os << "Verbose";
        case TraceLevel::Off:
            return os;
        default:
            return os;
    }
}

constexpr bool operator<=(TraceLevel lhs, TraceLevel rhs) noexcept
{
    return static_cast<unsigned char>(lhs) <= static_cast<unsigned char>(rhs);
}

template <TraceLevel level>
constexpr bool isActive{::detail::ActiveTraceLevel::value != TraceLevel::Off &&
                        static_cast<unsigned char>(level) <= static_cast<unsigned char>(::detail::ActiveTraceLevel::value)};

template <TraceLevel level, bool = isActive<level>>
struct log_impl;
template <TraceLevel level>
struct log_impl<level, false>
{
    template <typename... Ts>
    void operator()(char const* const, int const, Ts&&...) const noexcept
    {
    }
};

template <TraceLevel level>
struct log_impl<level, true>
{
    void print() const
    {
        std::cerr << "\n";
    }
    template <typename T, typename... Ts>
    void print(T&& msg, Ts&&... msgs) const
    {
        std::cerr << std::forward<T>(msg);
        print(std::forward<Ts>(msgs)...);
    }

    template <typename... Ts>
    void operator()(char const* const trace, int const line, Ts&&... msgs) const
    {
        std::cerr << level << " [" << trace << ", " << line << "]: ";
        print(std::forward<Ts>(msgs)...);
    }
};

}    // namespace detail

#define LT_LOG(LEVEL, ...)                                                                                             \
    ::detail::log_impl<::detail::TraceLevel::LEVEL>{}(__FILE__, __LINE__, __VA_ARGS__)

#else

namespace detail
{
inline void nop() noexcept
{
}
}    // namespace detail


#define LT_LOG(LEVEL, ...) ::detail::nop()

#endif /* ENABLE_TRACE_LOGS */

#endif /* TRACE_HEADER_GUARD_HPP_ */
