#include <gtest/gtest.h>

#include <hash_map/hash_map_bucket.hpp>

#include <iostream>
#include <forward_list>
#include <algorithm>
#include <array>
#include <string>
#include <vector>
#include <random>

namespace
{

TEST(HashMapBucketTest, default_constructed_bucket_is_empty)
{
    HashMapBucket<int, double> sut;
    ASSERT_TRUE(sut.empty());
}

class HashMapBucketInsertingModifiers : public ::testing::Test {
protected:
    HashMapBucket<int, double> sut{};

    HashMapBucketInsertingModifiers()
    {
        EXPECT_TRUE(sut.empty());
    }
};

TEST_F(HashMapBucketInsertingModifiers, push_front_adds_single_node)
{
    constexpr int key{42};
    constexpr double value{3.14};
    sut.push_front({key, value});
    ASSERT_FALSE(sut.empty());
    auto it{sut.begin()};
    ASSERT_NE(it, sut.end());
    ASSERT_EQ(it->first, key);
    ASSERT_EQ(it->second, value);
}

TEST_F(HashMapBucketInsertingModifiers, push_front_adds_single_node_rvalue)
{
    constexpr int key{42};
    constexpr double value{3.14};
    sut.push_front({int{key}, double{value}});
    ASSERT_FALSE(sut.empty());
    auto it{sut.begin()};
    ASSERT_NE(it, sut.end());
    ASSERT_EQ(it->first, key);
    ASSERT_EQ(it->second, value);
    constexpr int key2{11};
    constexpr double value2{2.124};
    sut.push_front({int{key2}, double{value2}});
    it = sut.begin();
    ASSERT_NE(it, sut.end());
    ASSERT_EQ(it->first, key2);
    ASSERT_EQ(it->second, value2);
}

TEST_F(HashMapBucketInsertingModifiers, insert_after_adds_a_single_node)
{
    constexpr int key{42};
    constexpr double value{3.14};
    auto const pos{sut.before_cbegin()};
    sut.insert_after(pos, {key, value});
    ASSERT_FALSE(sut.empty());
}

TEST_F(HashMapBucketInsertingModifiers, insert_after_range_adds_range_size_nodes)
{
    constexpr std::array<std::pair<int, double>, 5> range{{{1, 1.1}, {2, 2.2}, {3, 3.3}, {4, 4.4}, {5, 5.5}}};
    auto const pos{sut.before_cbegin()};
    sut.insert_after(pos, range.cbegin(), range.cend());
    auto it{sut.cbegin()};
    for (auto expected{range.cbegin()}; expected != range.cend(); ++expected, ++it)
    {
        ASSERT_EQ(expected->first, it->first);
        ASSERT_EQ(expected->second, it->second);
    }
}


class HashMapBucketErasingModifiers : public ::testing::Test {
public:
    static constexpr std::array<std::pair<int, double>, 5> init_{{{2, 2.2}, {5, 5.5}, {3, 3.3}, {1, 1.1}, {4, 4.4}}};
protected:
    HashMapBucket<int, double> sut{init_.cbegin(), init_.cend()};
};

TEST_F(HashMapBucketErasingModifiers, erase_removes_nodes_with_the_matching_key)
{
    auto const duplicate{init_[init_.size()/2]};
    sut.push_front(duplicate);  // make sure one of the elements is present in the list more than once
    auto const count_duplicate{std::count_if(
        sut.cbegin(), sut.cend(), [&duplicate](auto const& v) noexcept {return v.first == duplicate.first;}
    )};
    auto const count_removed{sut.erase(duplicate.first)};
    ASSERT_EQ(count_duplicate, count_removed);
    auto const removed_found{std::find_if(sut.cbegin(), sut.cend(),
        [key=duplicate.first](auto const& v) noexcept { return v.first == key; }
    )};
    ASSERT_EQ(removed_found, sut.cend());
}

class HashMapBucketErasingModifiersEdgeCaseParameterizedTest
    : public HashMapBucketErasingModifiers,
      public ::testing::WithParamInterface<std::pair<int, double>>
{
};

TEST_P(HashMapBucketErasingModifiersEdgeCaseParameterizedTest, erase_removes_nodes_with_matching_key_edge_case)
{
    auto const node_to_remove{GetParam()};
    auto const count_to_remove{std::count_if(
        sut.cbegin(), sut.cend(), [key=node_to_remove.first](auto const& v) noexcept { return v.first == key; }
    )};
    auto const count_removed{sut.erase(node_to_remove.first)};
    ASSERT_EQ(count_removed, count_to_remove);
    auto const removed_found{std::find_if(sut.cbegin(), sut.cend(),
        [key=node_to_remove.first](auto const& v) noexcept { return v.first == key; }
    )};
    ASSERT_EQ(removed_found, sut.cend());
}

INSTANTIATE_TEST_CASE_P(
    EraseEdgeCaseTest,
    HashMapBucketErasingModifiersEdgeCaseParameterizedTest,
    ::testing::Values(
        HashMapBucketErasingModifiersEdgeCaseParameterizedTest::init_.front(),
        HashMapBucketErasingModifiersEdgeCaseParameterizedTest::init_.back()
    )
);

} // namespace
