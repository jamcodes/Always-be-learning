#include "gtest/gtest.h"
#include "basic_tuple/basic_tuple.h"
#include <tuple>

namespace
{

class CtorsTest : public ::testing::Test {
};


TEST_F(CtorsTest, Empty)
{
    const Tuple<> tup{};
}

TEST_F(CtorsTest, SingleElem)
{
    Tuple<int> tup1(3);
    const auto ctup2 = makeTuple(2);
    static_assert(std::is_same_v<decltype(ctup2), const Tuple<int>>);
}

TEST_F(CtorsTest, TwoElem)
{
    Tuple<int, double> tup1(1, 3.14);
    const auto tup2 = makeTuple(1, 3.14);
    const auto tup3 = Tuple<int, double>{1, 3.14};
}

} // namespace
