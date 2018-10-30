#include "gtest/gtest.h"
#include "basic_tuple/basic_tuple.h"


namespace
{

class TestComparison : public ::testing::Test
{
protected:

    Tuple<> empty{};
    Tuple<int> i42{42};
    Tuple<double> d314{3.14};
};

TEST_F(TestComparison, TestEquals)
{
    ASSERT_EQ(empty, empty);
    ASSERT_EQ(i42, i42);
    ASSERT_EQ(d314, d314);
    const Tuple<int, double> t1{1, 2.2};
    const Tuple<int, double> t2 = t1;
    ASSERT_EQ(t1, t2);
}

TEST_F(TestComparison, TestNotEquals)
{
    ASSERT_NE(empty, i42);
    ASSERT_NE(i42, empty);
    ASSERT_NE(i42, d314);
    ASSERT_NE(d314, i42);
    const Tuple<int, double, char> t1{42, 3.14, 'a'};
    ASSERT_NE(t1, i42);
    ASSERT_NE(i42, t1);
}

TEST_F(TestComparison, TestLessThan)
{
    const Tuple<int> t0{0};
    ASSERT_LT(t0, i42);
    ASSERT_LT(t0, d314);
    ASSERT_LT(d314, i42);
    const Tuple<int, double> t2{42, 3.14};
    ASSERT_LT(i42, t2);
    const Tuple<int, double, char> t3{42, 3.14, 'a'};
    ASSERT_LT(t2,t3);
}

} // namespace
