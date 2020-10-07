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
    const double new_value{9.9};
    std::vector<std::pair<int, double>> expected(init.cbegin(), init.cend());
    expected.push_back({key, new_value});
    std::sort(expected.begin(), expected.end());
    expected.erase(std::remove(expected.begin(), expected.end(), kv));

    auto const res{sut.insert_or_assign(key, new_value)};
    ASSERT_FALSE(res.second);
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected.cbegin(), expected.cend(), Compare{}));
}

TEST_F(BinaryTreeInsertingModifiersTest, insert_or_assign_assigns_value_for_existing_key_rvalue)
{
    auto const kv{init.front()};
    const int key{kv.first};
    const double new_value{9.9};
    std::vector<std::pair<int, double>> expected(init.cbegin(), init.cend());
    expected.push_back({key, new_value});
    std::sort(expected.begin(), expected.end());
    expected.erase(std::remove(expected.begin(), expected.end(), kv));

    auto const res{sut.insert_or_assign(int{key}, new_value)};
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

TEST_F(BinaryTreeInsertingModifiersTest, try_emplace_returns_current_value_of_existing_key)
{
    auto const kv{init.front()};
    const int key{kv.first};
    const double new_value{9.9};
    std::vector<std::pair<int, double>> expected(init.cbegin(), init.cend());
    std::sort(expected.begin(), expected.end());

    auto const res{sut.try_emplace(key, new_value)};
    ASSERT_FALSE(res.second);
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected.cbegin(), expected.cend(), Compare{}));
}

class BinaryTreeErasingModifiersTest : public ::testing::Test {
protected:
    constexpr static std::array<std::pair<int, double>, 8> init{{{3, 3.3}, {5, 5.5}, {2, 2.2}, {1, 1.1}, {4, 4.4},
                                                                 {8, 8.8}, {6, 6.6}, {10, 10.10}}};
    constexpr static std::array<std::pair<int, double>, 8> init_inorder{
        {{1, 1.1}, {2, 2.2}, {3, 3.3}, {4, 4.4}, {5, 5.5}, {6, 6.6}, {8, 8.8}, {10, 10.10}}
    };

    struct Compare {
        template<typename T, typename U>
        bool operator()(T const& lhs, U const& rhs) const noexcept
        {
            return lhs.first == rhs.first && lhs.second == rhs.second;
        }
    };

    BinaryTreeErasingModifiersTest()
    {
        EXPECT_TRUE(std::equal(sut.cbegin(), sut.cend(), init_inorder.cbegin(), init_inorder.cend(), Compare{}));
    }

    BinaryTree<int, double> sut{init.cbegin(), init.cend()};
};

TEST_F(BinaryTreeErasingModifiersTest, erase_leaf_removes_node_preserves_sorting_order)
{
    // {1, 1.1}, {6, 6.6} and {10, 10.10} are leaf-nodes
    std::vector<std::pair<int, double>> expected{init_inorder.cbegin(), init_inorder.cend()};
    auto it{sut.cbegin()};
    constexpr std::pair<int, double> erase1{1, 1.1};
    constexpr std::pair<int, double> erase6{6, 6.6};
    constexpr std::pair<int, double> erase10{10, 10.10};
    ASSERT_EQ(it->first, erase1.first);
    ASSERT_EQ(it->second, erase1.second);
    sut.erase(it);
    expected.erase(std::remove(expected.begin(), expected.end(), erase1));
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected.cbegin(), expected.cend(), Compare{}));

    while (it != sut.cend() && it->first != erase6.first) {
        ++it;
    }
    ASSERT_NE(it, sut.cend());
    ASSERT_EQ(it->first, erase6.first);
    ASSERT_EQ(it->second, erase6.second);
    sut.erase(it);
    expected.erase(std::remove(expected.begin(), expected.end(), erase6));
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected.cbegin(), expected.cend(), Compare{}));

    while (it != sut.cend() && it->first != erase10.first) {
        ++it;
    }
    ASSERT_NE(it, sut.cend());
    ASSERT_EQ(it->first, erase10.first);
    ASSERT_EQ(it->second, erase10.second);
    sut.erase(it);
    expected.erase(std::remove(expected.begin(), expected.end(), erase10));
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected.cbegin(), expected.cend(), Compare{}));
}

TEST_F(BinaryTreeErasingModifiersTest, erase_smibranch_removes_node_preserves_sorting_order)
{
    // {2, 2.2} is a semi-branch
    std::vector<std::pair<int, double>> expected{init_inorder.cbegin(), init_inorder.cend()};
    constexpr std::pair<int, double> erase2{2, 2.2};
    auto it{sut.cbegin()};
    while (it != sut.cend() && it->first != erase2.first) {
        ++it;
    }
    ASSERT_NE(it, sut.cend());
    ASSERT_EQ(it->first, erase2.first);
    ASSERT_EQ(it->second, erase2.second);
    sut.erase(it);
    expected.erase(std::remove(expected.begin(), expected.end(), erase2));
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected.cbegin(), expected.cend(), Compare{}));
}

TEST_F(BinaryTreeErasingModifiersTest, erase_branch_removes_node_preserves_sorting_order)
{
    // {8, 8.8} is a semi-branch
    std::vector<std::pair<int, double>> expected{init_inorder.cbegin(), init_inorder.cend()};
    constexpr std::pair<int, double> erase8{8, 8.8};
    auto it{sut.cbegin()};
    while (it != sut.cend() && it->first != erase8.first) {
        ++it;
    }
    ASSERT_NE(it, sut.cend());
    ASSERT_EQ(it->first, erase8.first);
    ASSERT_EQ(it->second, erase8.second);
    sut.erase(it);
    expected.erase(std::remove(expected.begin(), expected.end(), erase8));
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected.cbegin(), expected.cend(), Compare{}));
}

TEST_F(BinaryTreeErasingModifiersTest, erase_root_removes_node_preserves_sorting_order)
{
    // {3, 3.3} is the root node
    std::vector<std::pair<int, double>> expected{init_inorder.cbegin(), init_inorder.cend()};
    constexpr std::pair<int, double> erase3{3, 3.3};
    auto it{sut.cbegin()};
    while (it != sut.cend() && it->first != erase3.first) {
        ++it;
    }
    ASSERT_NE(it, sut.cend());
    ASSERT_EQ(it->first, erase3.first);
    ASSERT_EQ(it->second, erase3.second);
    sut.erase(it);
    expected.erase(std::remove(expected.begin(), expected.end(), erase3));
    ASSERT_TRUE(std::equal(sut.cbegin(), sut.cend(), expected.cbegin(), expected.cend(), Compare{}));
}

class BinaryTreeLookupMembersTest : public ::testing::Test {
protected:
    constexpr static std::array<std::pair<int, double>, 8> init{{{3, 3.3}, {5, 5.5}, {2, 2.2}, {1, 1.1}, {4, 4.4},
                                                                 {8, 8.8}, {6, 6.6}, {10, 10.10}}};
    constexpr static std::array<std::pair<int, double>, 8> init_inorder{
        {{1, 1.1}, {2, 2.2}, {3, 3.3}, {4, 4.4}, {5, 5.5}, {6, 6.6}, {8, 8.8}, {10, 10.10}}
    };

    struct Compare {
        template<typename T, typename U>
        bool operator()(T const& lhs, U const& rhs) const noexcept
        {
            return lhs.first == rhs.first && lhs.second == rhs.second;
        }
    };

    BinaryTreeLookupMembersTest()
    {
        EXPECT_TRUE(std::equal(sut.cbegin(), sut.cend(), init_inorder.cbegin(), init_inorder.cend(), Compare{}));
    }

    BinaryTree<int, double> sut{init.cbegin(), init.cend()};
};

TEST_F(BinaryTreeLookupMembersTest, count_returns_nuber_of_nodes_with_equal_key)
{
    auto const search_key{init[2].first};
    auto const count_result{sut.count(search_key)};
    ASSERT_EQ(count_result, 1);

    int const nonexistant_key{42};
    auto const count_nonexistant_result{sut.count(nonexistant_key)};
    ASSERT_EQ(count_nonexistant_result, 0);
}

TEST_F(BinaryTreeLookupMembersTest, find_returns_an_iterator_to_a_node_with_equal_key)
{
    auto const search_key{init[2].first};
    auto it{sut.find(search_key)};
    ASSERT_NE(it, sut.end());
    ASSERT_EQ(it->first, search_key);
}

TEST_F(BinaryTreeLookupMembersTest, find_returns_end_iterator_if_key_is_not_found)
{
    int const unavailable_key{42};
    auto it{sut.find(unavailable_key)};
    ASSERT_EQ(it, sut.end());
}

} // namespace
