#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include "auxiliary.h"


template< class InputIt1, class InputIt2 >
constexpr bool lexicographical_compare( InputIt1 first1, InputIt1 last1,
                                        InputIt2 first2, InputIt2 last2 );

template< class InputIt1, class InputIt2, class Compare >
constexpr bool lexicographical_compare( InputIt1 first1, InputIt1 last1,
                                        InputIt2 first2, InputIt2 last2,
                                        Compare comp );

/**
 * Returns whether the elements in the first range are 'lexicographically less than' the elements
 * in the second range.
 * 1st overload compares using <
 * 2nd overload compares using binary predicate comp(el1, el2) which should return true
 * if el1 is less than el2
 * Lexicographically means that sequences are compared element by element untill:
 *  - elements are not equal -> result of their comparison is the entire result
 *  - one sequence has no more elements -> the sequence with less elements is less than the other
 *  - both have no more elements -> result of the comparison is false
 */

template<typename T, typename U>
void lexcompare_colls(const T& lhs, const U& rhs) noexcept
{
    if(std::lexicographical_compare(lhs.cbegin(), lhs.cend(),   // first range
                                    rhs.cbegin(), rhs.cend()))  // second range
    {
        std::cout << "coll1 is lexicographically less than coll2\n";
    } else {
        std::cout << "coll1 and coll2 are equal\n";
    }
}

int main()
{
    std::array<int, 11> coll1 = {1, 2, 11, 17, 24, 42, 3, 5, 9, 81, 49};
    std::vector<int> coll2(coll1.cbegin(), coll1.cend());
    print_elements(coll1, "coll1: ");
    print_elements(coll2, "coll2: ");
    lexcompare_colls(coll1, coll2);

    std::sort(coll1.begin(), coll1.end());
    print_elements(coll1, "coll1 sorted: ");
    print_elements(coll2, "coll2: ");
    lexcompare_colls(coll1, coll2);
}
