#pragma once

// Indirection is required here - if we want to concatenate actual value of preprocessor
// symbols we need them to be expanded first.
// Without the indirection "__COUNTER__" would be concatenated with the
// name of the variable, rather than the value of __COUNTER__
#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)

#if defined(__COUNTER__)
#define ANONYMOUS_VARIABLE(str) \
    CONCATENATE(str, __COUNTER__)
#else
#define ANONYMOUS_VARIABLE(str) \
    CONCATENATE(str, __LINE__)
#endif
