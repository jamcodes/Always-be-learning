#include "gtest/gtest.h"
#include "basic_tuple/basic_tuple.h"



namespace
{

class GetTest : public ::testing::Test {
};


TEST_F(GetTest, SinleElem)
{
    const Tuple<int> tup{42};
    int i = GET<0>(tup);
}

} // namespace
