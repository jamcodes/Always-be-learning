#include <gtest/gtest.h>

#include <binary_tree/binary_tree.hpp>

#include <array>
#include <memory>
#include <utility>

namespace
{

TEST(BinaryTreeTest, default_constructed_bt_is_empty)
{
    BinaryTree<int, double> sut{};
    ASSERT_TRUE(sut.empty());
}

TEST(BinaryTreeTest, range_constructed_bt_is_not_empty)
{
    std::array<std::pair<int, double>, 4> init{{{1, 1.1}, {2, 2.2}, {3, 3.3}, {4, 4.4}}};
    BinaryTree<int, double> sut{init.cbegin(), init.cend()};
    ASSERT_FALSE(sut.empty());

    std::size_t counter{0};
    auto sut_it{sut.cbegin()};
    for (auto it{init.cbegin()}; sut_it != sut.cend() && it != init.cend(); ++sut_it, ++it)
    {
        ASSERT_EQ(sut_it->first, it->first);
        ASSERT_EQ(sut_it->second, it->second);
        ++counter;
    }
    ASSERT_EQ(counter, init.size());
}

TEST(BinaryTreeTest, copy_constructed_bt_is_equal_to_source_bt)
{
    std::array<std::pair<int, double>, 4> init{{{1, 1.1}, {2, 2.2}, {3, 3.3}, {4, 4.4}}};
    BinaryTree<int, double> source{init.cbegin(), init.cend()};
    ASSERT_FALSE(source.empty());

    BinaryTree<int, double> sut{source};
    ASSERT_FALSE(sut.empty());
    ASSERT_TRUE(std::equal(source.cbegin(), source.cend(), sut.cbegin(), sut.cend()));
}

TEST(BinaryTreeTest, move_constructed_bt_is_equal_to_premove_source_and_postmove_source_is_empty)
{
    constexpr static std::array<std::pair<int, double>, 5> init{{{5, 5.5}, {2, 2.2}, {3, 3.3}, {1, 1.1}, {4, 4.4}}};
    constexpr static std::array<std::pair<int, double>, 5> expected_inorder{
        {{1, 1.1}, {2, 2.2}, {3, 3.3}, {4, 4.4}, {5, 5.5}}
    };
    BinaryTree<int, double> source{init.cbegin(), init.cend()};
    auto const cmp{
        [](auto const& lhs, auto const& rhs)noexcept { return lhs.first == rhs.first && lhs.second == rhs.second; }
    };
    ASSERT_TRUE(std::equal(source.cbegin(), source.cend(), expected_inorder.cbegin(), expected_inorder.cend(), cmp));

    BinaryTree<int, double> sut{std::move(source)};
    ASSERT_FALSE(sut.empty());
    ASSERT_TRUE(source.empty());
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected_inorder.cbegin(), expected_inorder.cend(), cmp));
}


class BinaryTreeInsertingModifiersTest : public ::testing::Test {
protected:
    constexpr static std::array<std::pair<int, double>, 4> init{{{5, 5.5}, {2, 2.2}, {1, 1.1}, {4, 4.4}}};
    constexpr static std::array<std::pair<int, double>, 4> init_inorder{
        {{1, 1.1}, {2, 2.2}, {4, 4.4}, {5, 5.5}}
    };

    struct Compare {
        template<typename T, typename U>
        bool operator()(T const& lhs, U const& rhs) const noexcept
        {
            return lhs.first == rhs.first && lhs.second == rhs.second;
        }
    };

    BinaryTreeInsertingModifiersTest()
    {
        EXPECT_TRUE(std::equal(sut.cbegin(), sut.cend(), init_inorder.cbegin(), init_inorder.cend(), Compare{}));
    }

    BinaryTree<int, double> sut{init.cbegin(), init.cend()};
};

TEST_F(BinaryTreeInsertingModifiersTest, insert_adds_node_preserving_sort_order)
{
    constexpr std::pair<const int, double> value{3, 3.3};
    std::vector<std::pair<int, double>> expected(init.cbegin(), init.cend());
    expected.push_back(value);
    std::sort(expected.begin(), expected.end());

    sut.insert(value);
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected.cbegin(), expected.cend(), Compare{}));
}

TEST_F(BinaryTreeInsertingModifiersTest, insert_adds_node_preserving_sort_order_rvalue)
{
    constexpr std::pair<const int, double> value{3, 3.3};
    std::vector<std::pair<int, double>> expected(init.cbegin(), init.cend());
    expected.push_back(value);
    std::sort(expected.begin(), expected.end());

    sut.insert({3, 3.3});
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected.cbegin(), expected.cend(), Compare{}));
}

TEST_F(BinaryTreeInsertingModifiersTest, insert_adds_node_preserving_sort_order_convertible)
{
    constexpr std::pair<int, double> value{3, 3.3};
    std::vector<std::pair<int, double>> expected(init.cbegin(), init.cend());
    expected.push_back(value);
    std::sort(expected.begin(), expected.end());

    sut.insert(value);
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected.cbegin(), expected.cend(), Compare{}));
}

TEST_F(BinaryTreeInsertingModifiersTest, insert_range_adds_nodes_preserving_sort_order)
{
    std::array<std::pair<int, double>, 3> values{{{3, 3.3}, {7, 7.7}, {0, 0.0}}};
    std::vector<std::pair<int, double>> expected(init.cbegin(), init.cend());
    expected.insert(expected.cend(), values.cbegin(), values.cend());
    std::sort(expected.begin(), expected.end());

    sut.insert(values.cbegin(), values.cend());
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected.cbegin(), expected.cend(), Compare{}));
}

TEST_F(BinaryTreeInsertingModifiersTest, emplace_adds_node_preserving_sort_order)
{
    constexpr std::pair<const int, double> value{3, 3.3};
    std::vector<std::pair<int, double>> expected(init.cbegin(), init.cend());
    expected.push_back(value);
    std::sort(expected.begin(), expected.end());

    sut.emplace(3, 3.3);
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected.cbegin(), expected.cend(), Compare{}));
}

TEST_F(BinaryTreeInsertingModifiersTest, insert_or_assign_adds_a_node_for_new_key)
{
    constexpr int key{3};
    constexpr double value{3.3};
    std::vector<std::pair<int, double>> expected(init.cbegin(), init.cend());
    expected.push_back({key, value});
    std::sort(expected.begin(), expected.end());

    sut.insert_or_assign(key, value);
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected.cbegin(), expected.cend(), Compare{}));
}

TEST_F(BinaryTreeInsertingModifiersTest, insert_or_assign_adds_a_node_for_new_rvalue_key)
{
    constexpr int key{3};
    constexpr double value{3.3};
    std::vector<std::pair<int, double>> expected(init.cbegin(), init.cend());
    expected.push_back({key, value});
    std::sort(expected.begin(), expected.end());

    auto const res{sut.insert_or_assign(3, value)};
    ASSERT_TRUE(res.second);
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected.cbegin(), expected.cend(), Compare{}));
}

TEST_F(BinaryTreeInsertingModifiersTest, insert_or_assign_assigns_value_for_existing_key)
{
    auto const kv{init.front()};
    const int key{kv.first};
    const double value{kv.second};
    std::vector<std::pair<int, double>> expected(init.cbegin(), init.cend());
    expected.push_back({key, value});
    std::sort(expected.begin(), expected.end());
    expected.erase(std::unique(expected.begin(), expected.end()), expected.end());

    auto const res{sut.insert_or_assign(key, value)};
    ASSERT_FALSE(res.second);
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected.cbegin(), expected.cend(), Compare{}));
}

TEST_F(BinaryTreeInsertingModifiersTest, insert_or_assign_assigns_value_for_existing_key_rvalue)
{
    auto const kv{init.front()};
    const int key{kv.first};
    const double value{kv.second};
    std::vector<std::pair<int, double>> expected(init.cbegin(), init.cend());
    expected.push_back({key, value});
    std::sort(expected.begin(), expected.end());
    expected.erase(std::unique(expected.begin(), expected.end()), expected.end());

    auto const res{sut.insert_or_assign(int{key}, value)};
    ASSERT_FALSE(res.second);
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected.cbegin(), expected.cend(), Compare{}));
}

TEST_F(BinaryTreeInsertingModifiersTest, insert_or_assign_hint_inserts_value_at_or_below_hint)
{
    constexpr int key{3};
    constexpr double value{3.3};
    std::vector<std::pair<int, double>> expected(init.cbegin(), init.cend());
    expected.push_back({key, value});
    std::sort(expected.begin(), expected.end());

    auto it{std::next(sut.cbegin())};
    ASSERT_EQ(it->first, 2);
    sut.insert_or_assign(it, key, value);
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected.cbegin(), expected.cend(), Compare{}));
}

TEST_F(BinaryTreeInsertingModifiersTest, try_emplace_adds_a_node_for_new_key)
{
    constexpr int key{3};
    constexpr double value{3.3};
    std::vector<std::pair<int, double>> expected(init.cbegin(), init.cend());
    expected.push_back({key, value});
    std::sort(expected.begin(), expected.end());

    sut.try_emplace(key, std::move(value));
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected.cbegin(), expected.cend(), Compare{}));
}

// TEST_F(BinaryTreeInsertingModifiersTest, try_emplace_returns_current_value_of_existing_key)
// {
//     auto const kv{init.front()};
//     const int key{kv.first};
//     const double value{kv.second};
//     std::vector<std::pair<int, double>> expected(init.cbegin(), init.cend());
//     expected.push_back({key, value});
//     std::sort(expected.begin(), expected.end());
//     expected.erase(std::unique(expected.begin(), expected.end()), expected.end());

//     auto const res{sut.insert_or_assign(key, value)};
//     ASSERT_FALSE(res.second);
//     ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected.cbegin(), expected.cend(), Compare{}));
// }

} // namespace
