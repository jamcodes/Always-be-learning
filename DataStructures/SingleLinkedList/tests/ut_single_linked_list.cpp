#include <gtest/gtest.h>

#include <single_linked_list/single_linked_list.hpp>

#include <iostream>
#include <forward_list>
#include <algorithm>
#include <array>
#include <string>

namespace
{

TEST(SingleLinkedListTest, default_constructed_sll_is_empty)
{
    // construct with trivial type
    SingleLinkedList<int> sut{};
    EXPECT_TRUE(sut.empty());
    // construct with non-trivial type
    SingleLinkedList<std::string> sut2{};
    EXPECT_TRUE(sut2.empty());
}

TEST(SingleLinkedListTest, non_default_constructed_sll_is_not_empty)
{
    SingleLinkedList<int> sut{42};
    EXPECT_FALSE(sut.empty());
    SingleLinkedList<std::string> sut2{2, std::string("some non inlined string maybe")};
    EXPECT_FALSE(sut2.empty());
}

TEST(SingleLinkedListTest, copy_ctor)
{
    SingleLinkedList<int> sut{2, 42};
    SingleLinkedList<int> sut_copy{sut};
    EXPECT_FALSE(sut.empty());
    EXPECT_FALSE(sut_copy.empty());
}

TEST(SingleLinkedListTest, move_ctor)
{
    SingleLinkedList<int> sut{2, 42};
    EXPECT_FALSE(sut.empty());
    SingleLinkedList<int> sut_move_constructed{std::move(sut)};
    EXPECT_FALSE(sut_move_constructed.empty());
    EXPECT_TRUE(sut.empty());
}

TEST(SingleLinkedListTest, clear_empties_the_sll)
{
    SingleLinkedList<int> sut{11, 42};
    EXPECT_FALSE(sut.empty());
    sut.clear();
    EXPECT_TRUE(sut.empty());
}

TEST(SingleLinkedListTest, swap_exchanges_heads)
{
    constexpr std::array<int, 4> elems_a{1, 2, 3, 4};
    constexpr std::array<int, 4> elems_b{99, 88, 77, 66};
    SingleLinkedList<int> sll_a(elems_a.cbegin(), elems_a.cend());
    SingleLinkedList<int> sll_b(elems_b.cbegin(), elems_b.cend());

    ASSERT_TRUE(std::equal(elems_a.cbegin(), elems_a.cend(), sll_a.cbegin(), sll_a.cend()));
    ASSERT_TRUE(std::equal(elems_b.cbegin(), elems_b.cend(), sll_b.cbegin(), sll_b.cend()));

    sll_a.swap(sll_b);
    ASSERT_TRUE(std::equal(elems_a.cbegin(), elems_a.cend(), sll_b.cbegin(), sll_b.cend()));
    ASSERT_TRUE(std::equal(elems_b.cbegin(), elems_b.cend(), sll_a.cbegin(), sll_a.cend()));

}


class SingleLinkedListInsertingModifiersTest : public ::testing::Test
{
protected:
    SingleLinkedList<std::string> sut{};
    SingleLinkedList<int> tsut{};
};

TEST_F(SingleLinkedListInsertingModifiersTest, insert_after_lvalue)
{
    constexpr int value42{42};
    constexpr int value11{11};
    auto it = tsut.insert_after(tsut.before_cbegin(), value42);
    ASSERT_FALSE(tsut.empty());
    ASSERT_EQ(*it, value42);
    auto it2 = tsut.insert_after(it, value11);
    ASSERT_EQ(*it2, value11);
    ASSERT_EQ(++it, it2);
    ASSERT_EQ(++it, tsut.cend());
}

TEST_F(SingleLinkedListInsertingModifiersTest, insert_after_rvalue)
{
    auto it = tsut.insert_after(tsut.before_cbegin(), 42);
    ASSERT_FALSE(tsut.empty());
    ASSERT_EQ(*it, 42);
    auto it2 = tsut.insert_after(it, 11);
    ASSERT_EQ(*it2, 11);
    ASSERT_EQ(++it, it2);
    ASSERT_EQ(++it, tsut.cend());
}

TEST_F(SingleLinkedListInsertingModifiersTest, insert_after_count)
{
    auto it = tsut.insert_after(tsut.before_cbegin(), 3, 42);
    ASSERT_FALSE(tsut.empty());
    ASSERT_EQ(*it, 42);

    auto b{tsut.cbegin()};
    int count{0};
    while (b != tsut.cend()) {
        ASSERT_EQ(*b, 42);
        ++count;
        ++b;
    }
    ASSERT_EQ(count, 3);
}

TEST_F(SingleLinkedListInsertingModifiersTest, insert_after_range)
{
    std::array<int, 3> range{1, 2, 3};
    auto it = tsut.insert_after(tsut.before_cbegin(), range.cbegin(), range.cend());
    ASSERT_FALSE(tsut.empty());
    ASSERT_EQ(*it, range.back());

    auto b{tsut.cbegin()};
    auto rb{range.cbegin()};

    while (b != tsut.cend() && rb != range.cend()) {
        ASSERT_EQ(*b, *rb);
        ++b;
        ++rb;
    }
    ASSERT_EQ(b, tsut.cend());
    ASSERT_EQ(rb, range.cend());
    ASSERT_EQ(range.size(), 3);
}

TEST_F(SingleLinkedListInsertingModifiersTest, emplace_after_adds_node)
{
    const std::string expected_str(3, 'A');
    // Forwarding the values to the constructor of Node causes a sign-conversion error here - int converted to uint.
    auto it = sut.emplace_after(sut.before_cbegin(), 3u, 'A');    // "AAA"
    ASSERT_FALSE(sut.empty());
    ASSERT_EQ(*it, expected_str);
}

TEST_F(SingleLinkedListInsertingModifiersTest, push_front_adds_node_to_the_front_lvalue)
{
    constexpr int value42{42};
    constexpr int value11{11};
    tsut.push_front(value42);
    ASSERT_FALSE(tsut.empty());
    ASSERT_EQ(*tsut.cbegin(), value42);
    tsut.push_front(value11);
    ASSERT_EQ(*tsut.cbegin(), value11);
}

TEST_F(SingleLinkedListInsertingModifiersTest, push_front_adds_node_to_the_front_rvalue)
{
    const std::string expected_str1(3, 'A');
    const std::string expected_str2(4, 'B');
    sut.push_front(std::string(3, 'A'));
    ASSERT_FALSE(sut.empty());
    ASSERT_EQ(*sut.cbegin(), expected_str1);
    sut.push_front(std::string(4, 'B'));
    ASSERT_EQ(*sut.cbegin(), expected_str2);
}

TEST_F(SingleLinkedListInsertingModifiersTest, emplace_front_adds_node_to_the_front)
{
    const std::string expected_str1(3, 'A');
    const std::string expected_str2(4, 'B');
    auto s1_ref = sut.emplace_front(3u, 'A');
    ASSERT_FALSE(sut.empty());
    ASSERT_EQ(*sut.cbegin(), expected_str1);
    ASSERT_EQ(s1_ref, expected_str1);
    auto s2_ref = sut.emplace_front(4u, 'B');
    ASSERT_EQ(*sut.cbegin(), expected_str2);
    ASSERT_EQ(s2_ref, expected_str2);
}

TEST_F(SingleLinkedListInsertingModifiersTest, erase_after_removes_node)
{
    tsut.push_front(1);
    tsut.push_front(2);
    tsut.push_front(3);
    ASSERT_FALSE(tsut.empty());
    ASSERT_EQ(*tsut.cbegin(), 3);
    auto it = tsut.erase_after(tsut.before_cbegin());
    ASSERT_EQ(*it, 2);
    ASSERT_EQ(*tsut.cbegin(), 2);
    auto it2 = tsut.erase_after(it);
    ASSERT_EQ(*tsut.cbegin(), 2);
    ASSERT_EQ(it2, tsut.cend());
}

TEST_F(SingleLinkedListInsertingModifiersTest, erase_after_on_last_element_is_noop)
{
    tsut.push_front(1);
    auto it = tsut.erase_after(tsut.cbegin());
    ASSERT_FALSE(tsut.empty());
    ASSERT_EQ(it, tsut.cend());
}

TEST_F(SingleLinkedListInsertingModifiersTest, erase_after_on_end_is_noop)
{
    tsut.push_front(1);
    auto it = tsut.erase_after(tsut.cend());
    ASSERT_FALSE(tsut.empty());
    ASSERT_EQ(it, tsut.cend());
}

class SingleLinkedListRemovingModifiersTest : public ::testing::Test {
protected:
    using Node = SLL_Node<int>;

    std::array<int, 5> ints_{1, 2, 3, 4, 5};
    SingleLinkedList<int> sut{ints_.cbegin(), ints_.cend()};
    const std::ptrdiff_t initial_count{std::count_if(sut.cbegin(), sut.cend(), [](auto&&)noexcept{return true;})};

    SingleLinkedListRemovingModifiersTest()
    {
        EXPECT_EQ(initial_count, ints_.size());
    }
};

TEST_F(SingleLinkedListRemovingModifiersTest, erase_after_range_removes_nodes)
{
    // Remove elements at indices [1, 3]
    auto first{sut.cbegin()};    // The first element of the range is not removed
    auto last{std::next(first, 4)};  // one-past the element to be removed
    ASSERT_EQ(*first, ints_[0]);
    ASSERT_EQ(*last, ints_[4]);

    auto it = sut.erase_after(first, last); // should return last
    ASSERT_EQ(*last, *it);

    // only two elements should remain:
    auto const count = std::count_if(sut.cbegin(), sut.cend(), [](auto&&) noexcept { return true; });
    ASSERT_EQ(count, 2);
    auto p = sut.cbegin();
    ASSERT_EQ(*p, ints_.front());
    ++p;
    ASSERT_EQ(*p, ints_.back());
    ++p;
    ASSERT_EQ(p, sut.cend());
}

TEST_F(SingleLinkedListRemovingModifiersTest, erase_after_with_zero_range_is_noop)
{
    auto first{sut.cbegin()};
    auto last{first};

    // erase with range first == last
    auto it = sut.erase_after(first, last);
    ASSERT_EQ(it, last);
    ASSERT_EQ(sut.cbegin(), first);
    auto count_after_erase{std::count_if(sut.cbegin(), sut.cend(), [](auto&&)noexcept{return true;})};
    ASSERT_EQ(count_after_erase, initial_count);

    ++last;
    // erase with range last == (++first)
    auto it2 = sut.erase_after(first, last);
    ASSERT_EQ(it2, last);
    ASSERT_EQ(sut.cbegin(), first);
    auto count_after_erase2{std::count_if(sut.cbegin(), sut.cend(), [](auto&&)noexcept{return true;})};
    ASSERT_EQ(count_after_erase2, initial_count);

}

TEST_F(SingleLinkedListRemovingModifiersTest, pop_front_removes_first_element)
{
    ASSERT_EQ(sut.front(), ints_.front());

    sut.pop_front();
    ASSERT_EQ(sut.front(), ints_[1]);
    auto const after_pop_count{std::count_if(sut.cbegin(), sut.cend(), [](auto&&)noexcept{return true;})};
    ASSERT_EQ(after_pop_count, initial_count - 1);
}

TEST_F(SingleLinkedListRemovingModifiersTest, pop_front_on_empty_list_is_noop)
{
    SingleLinkedList<int> empty_sll{};
    ASSERT_TRUE(empty_sll.empty());
    empty_sll.pop_front();
    ASSERT_TRUE(empty_sll.empty());
}

TEST_F(SingleLinkedListRemovingModifiersTest, resize_removes_elements_if_requested_size_smaller_than_current)
{
    auto const requested_size{static_cast<std::size_t>(initial_count - 2)};
    ASSERT_LT(requested_size, initial_count);
    sut.resize(requested_size);
    auto const count_after_resize{std::count_if(sut.cbegin(), sut.cend(), [](auto&&)noexcept{return true;})};
    ASSERT_EQ(requested_size, count_after_resize);
}

TEST_F(SingleLinkedListRemovingModifiersTest, resize_inserts_elements_if_requested_size_is_larger_than_current)
{
    auto const requested_size{static_cast<std::size_t>(initial_count + 2)};
    ASSERT_GT(requested_size, initial_count);
    sut.resize(requested_size);
    auto const count_after_resize{std::count_if(sut.cbegin(), sut.cend(), [](auto&&)noexcept{return true;})};
    ASSERT_EQ(requested_size, count_after_resize);

    auto it = std::next(sut.cbegin(), initial_count);
    constexpr decltype(sut)::value_type value_initialized_elem{};
    while (it != sut.cend()) {
        ASSERT_EQ(*it, value_initialized_elem);
        ++it;
    }
}

TEST_F(SingleLinkedListRemovingModifiersTest, resize_inserts_given_value_if_requested_size_is_larger_than_current)
{
    auto const requested_size{static_cast<std::size_t>(initial_count + 2)};
    constexpr decltype(sut)::value_type requqsted_value{42};
    ASSERT_GT(requested_size, initial_count);
    sut.resize(requested_size, requqsted_value);
    auto const count_after_resize{std::count_if(sut.cbegin(), sut.cend(), [](auto&&)noexcept{return true;})};
    ASSERT_EQ(requested_size, count_after_resize);

    auto it = std::next(sut.cbegin(), initial_count);
    while (it != sut.cend()) {
        ASSERT_EQ(*it, requqsted_value);
        ++it;
    }
}

TEST_F(SingleLinkedListRemovingModifiersTest, resize_is_noop_if_requested_size_equals_current)
{
    sut.resize(static_cast<std::size_t>(initial_count));
    auto const count_after_resize{std::count_if(sut.cbegin(), sut.cend(), [](auto&&)noexcept{return true;})};
    ASSERT_EQ(initial_count, count_after_resize);
}

TEST(SingleLinkedListMergeTest, merge_moves_nodes_into_merging_list)
{
    constexpr std::array<int, 5> elems_a{0, 2, 4, 6, 8};
    constexpr std::array<int, 5> elems_b{1, 3, 5, 7, 9};
    constexpr std::array<int, 10> expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    SingleLinkedList<int> sll_a(elems_a.cbegin(), elems_a.cend());
    SingleLinkedList<int> sll_b(elems_b.cbegin(), elems_b.cend());

    sll_a.merge(sll_b);
    ASSERT_TRUE(sll_b.empty());
    ASSERT_TRUE(std::equal(sll_a.cbegin(), sll_a.cend(), expected.cbegin()));
}

TEST(SingleLinkedListMergeTest, merge_with_an_empty_other_is_noop)
{
    constexpr std::array<int, 5> elems_a{0, 2, 4, 6, 8};
    SingleLinkedList<int> sll_a(elems_a.cbegin(), elems_a.cend());
    SingleLinkedList<int> sll_b{};

    sll_a.merge(sll_b);
    ASSERT_TRUE(sll_b.empty());
    ASSERT_TRUE(std::equal(sll_a.cbegin(), sll_a.cend(), elems_a.cbegin()));
}

TEST(SingleLinkedListMergeTest, merge_on_an_empty_list_swaps)
{
    SingleLinkedList<int> sll_a{};
    constexpr std::array<int, 5> elems_b{0, 2, 4, 6, 8};
    SingleLinkedList<int> sll_b(elems_b.cbegin(), elems_b.cend());

    sll_a.merge(sll_b);
    std::cout << "sll_a: ";
    for (auto&& n : sll_a) { std::cout << n << ", "; }
    std::cout << "\n";
    std::cout << "\nsll_b: ";
    for (auto&& n : sll_b) { std::cout << n << ", "; }
    std::cout << "\n";

    ASSERT_TRUE(sll_b.empty());
    ASSERT_TRUE(std::equal(sll_a.cbegin(), sll_a.cend(), elems_b.cbegin()));
}

} // namespace
