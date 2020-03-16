#include <iostream>

bool check_for_error_A(int64_t);
void handle_error_A(int64_t);
bool check_for_error_B(int64_t);
void handle_error_B(int64_t);
bool check_for_error_C(int64_t);
void handle_error_C(int64_t);
void handle_error(int64_t);
void do_what_we_actually_want();

// When working on code that's on the hotpath it's advantagous to make sure the error-handling
// code is not inlined. This can be achieved by:
// * Putting the hot-path condition first in the `if` statement
// * having as little branches as possible - prefferably a single if-else
// * grouping erro-handling code together in a separate function, that's non-inline
// * using compiler intrinsics like __builtin_expect to mark the likely/unlikely branches
// * since C++20 [[likely]] [[unlikely]]


void foo_slow()
{
    // This is slower because the error handling checks are inlined and more likely
    // to the hardware branch predictor
    int64_t error_flags{0};
    // ...
    if (check_for_error_A(error_flags))
        handle_error_A(error_flags);
    else if (check_for_error_B(error_flags))
        handle_error_B(error_flags);
    else if (check_for_error_C(error_flags))
        handle_error_C(error_flags);
    else
        do_what_we_actually_want();
}

void foo_fast()
{
    int64_t error_flags{0};
    // ...
    if (!error_flags)
        do_what_we_actually_want();
    else
        handle_error(error_flags);
}


int main()
{
}
