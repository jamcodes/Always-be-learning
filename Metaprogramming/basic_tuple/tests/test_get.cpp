#include "gtest/gtest.h"
#include "basic_tuple/basic_tuple.h"



namespace
{

class GetTest : public ::testing::Test {
};


TEST_F(GetTest, SinleElem)
{
    const Tuple<int> tup{42};
    int i = Get<0>(tup);
    ASSERT_EQ(i, 42);
}

TEST_F(GetTest, TwoElem)
{
    constexpr int i = 42;
    constexpr double d = 3.14;
    Tuple<int, double> tup(i, d);
    const auto t0 = Get<0>(tup);
    const auto t1 = Get<1>(tup);
    ASSERT_EQ(i, t0);
    ASSERT_EQ(d, t1);
}

} // namespace
