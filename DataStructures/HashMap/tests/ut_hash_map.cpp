#include <gtest/gtest.h>

#include <hash_map/hash_map.hpp>

#include <array>
#include <algorithm>
#include <utility>
#include <memory>

namespace
{

TEST(HashMapTest, constructor)
{
    HashMap<int, double> sut{};
    ASSERT_TRUE(sut.empty());
}

TEST(HashMapTest, constructor_custom_bucket_count)
{
    HashMap<int, double> sut{11};
    ASSERT_TRUE(sut.empty());
}

TEST(HashMapTest, constructor_from_range)
{
    constexpr std::array<std::pair<int, double>, 5> init{{{2, 2.2}, {3, 3.3}, {1, 1.1}, {5, 5.5}, {4, 4.4}}};
    HashMap<int, double> sut{init.cbegin(), init.cend()};
    ASSERT_EQ(init.size(), sut.size());
    auto it = sut.cbegin();
    auto end = sut.cend();
    ASSERT_NE(it, end);
    ASSERT_TRUE(std::is_permutation(init.cbegin(), init.cend(), sut.cbegin(), sut.cend(),
        [](auto const& lhs, auto const& rhs)noexcept{ return lhs.first == rhs.first && rhs.first == rhs.first; })
    );
}

class HashMapCopyMoveTests : public ::testing::Test {
public:
    static constexpr std::array<std::pair<int, double>, 5> init{{{2, 2.2}, {3, 3.3}, {1, 1.1}, {5, 5.5}, {4, 4.4}}};

protected:
    HashMap<int, double> sut{init.cbegin(), init.cend()};
};

TEST_F(HashMapCopyMoveTests, copy_constructor)
{
    HashMap<int, double> sut_copy{sut};
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), sut_copy.cbegin(), sut_copy.cend()));
}

TEST_F(HashMapCopyMoveTests, move_constructor)
{
    HashMap<int, double> sut_copy{sut};
    HashMap<int, double> sut_moved{std::move(sut_copy)};

    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), sut_moved.cbegin(), sut_moved.cend()));
}

TEST_F(HashMapCopyMoveTests, copy_assignment)
{
    static constexpr std::array<std::pair<int, double>, 2> vals{{{9, 9.9}, {11, 11.11}}};
    HashMap<int, double> assigned{vals.cbegin(), vals.cend()};
    ASSERT_FALSE(std::equal(sut.cbegin(), sut.cend(), assigned.cbegin(), assigned.cend()));
    assigned = sut;
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), assigned.cbegin(), assigned.cend()));
}

TEST_F(HashMapCopyMoveTests, move_assignment)
{
    static constexpr std::array<std::pair<int, double>, 2> vals{{{9, 9.9}, {11, 11.11}}};
    HashMap<int, double> assigned{vals.cbegin(), vals.cend()};
    HashMap<int, double> sut_copy{sut};
    ASSERT_FALSE(std::equal(sut.cbegin(), sut.cend(), assigned.cbegin(), assigned.cend()));
    assigned = std::move(sut_copy);
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), assigned.cbegin(), assigned.cend()));
    ASSERT_TRUE(sut_copy.empty());
}

class HashMapCapacityTests : public ::testing::Test {
protected:
    HashMap<int, double> sut;
};

TEST_F(HashMapCapacityTests, empty_observer_test)
{
    ASSERT_TRUE(sut.empty());
}

TEST_F(HashMapCapacityTests, size_observer_test)
{
    
}

} // namespace
