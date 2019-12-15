#pragma once

struct source_location {
    const char* file;
    unsigned line;
};

#define CUR_SRC_LOC() source_location{__FILE__, __LINE__}

template<unsigned Level>
inline constexpr auto ContractLevel{std::integral_constant<unsigned, Level>::value};

template<unsigned Level>
inline constexpr auto IsActive{Level <= ContractLevel};

template<typename Handler, bool = IsActive<Handler::level>> struct ContractAssert {
    template<typename Expr, typename... Args>
    static void do_assert(Expr&& expr, const char* const msg, source_location&& loc)
};
