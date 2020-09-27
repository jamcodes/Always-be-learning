#include <gtest/gtest.h>

#include <single_linked_list/single_linked_list.hpp>

#include <string>
#include <type_traits>
#include <utility>

#include <utils/is_detected.h>

namespace
{

class SllIteratorTest : public ::testing::Test {
protected:
    using Node = SLL_Node<int>;
    Node node1_{1};
    Node node2_{2};
    Node node3_{3};

    static constexpr void link_nodes(Node* lhs, Node* rhs) noexcept
    {
        lhs->next = rhs;
    }

    SllIteratorTest()
    {
        link_nodes(&node1_, &node2_);
        link_nodes(&node2_, &node3_);
    }
};

TEST_F(SllIteratorTest, dereference_returns_reference_to_data)
{
    SLL_Iterator<int> sut{&node1_};
    ASSERT_EQ(*sut, node1_.data);
}

TEST_F(SllIteratorTest, const_iterator_returns_const_reference)
{
    SLL_ConstIterator<int> sut{&node1_};
    ASSERT_TRUE(std::is_const_v<std::remove_reference_t<decltype(*sut)>>);
}

TEST_F(SllIteratorTest, preincrement_mutates)
{
    SLL_Iterator<int> sut{&node1_};
    ASSERT_EQ(*sut, node1_.data);
    auto& ref{++sut};
    ASSERT_EQ(*sut, node2_.data);
    ASSERT_EQ(*ref, *sut);
    auto& ref2{++sut};
    ASSERT_EQ(*sut, node3_.data);
    ASSERT_EQ(*ref2, *sut);
}

TEST_F(SllIteratorTest, postincrement_mutates_and_returns_a_copy)
{
    SLL_Iterator<int> sut{&node1_};
    ASSERT_EQ(*sut, node1_.data);
    auto cpy{sut++};
    ASSERT_EQ(*sut, node2_.data);
    ASSERT_EQ(*cpy, node1_.data);
    auto cpy2{sut++};
    ASSERT_EQ(*sut, node3_.data);
    ASSERT_EQ(*cpy2, node2_.data);
}

TEST_F(SllIteratorTest, const_preincrement_mutates)
{
    SLL_ConstIterator<int> sut{&node1_};
    ASSERT_EQ(*sut, node1_.data);
    auto& ref{++sut};
    ASSERT_EQ(*sut, node2_.data);
    ASSERT_EQ(*ref, *sut);
    auto& ref2{++sut};
    ASSERT_EQ(*sut, node3_.data);
    ASSERT_EQ(*ref2, *sut);
}

TEST_F(SllIteratorTest, const_postincrement_mutates_and_returns_a_copy)
{
    SLL_ConstIterator<int> sut{&node1_};
    ASSERT_EQ(*sut, node1_.data);
    auto cpy{sut++};
    ASSERT_EQ(*sut, node2_.data);
    ASSERT_EQ(*cpy, node1_.data);
    auto cpy2{sut++};
    ASSERT_EQ(*sut, node3_.data);
    ASSERT_EQ(*cpy2, node2_.data);
}

TEST_F(SllIteratorTest, non_const_iterator_implicitly_converts_to_const)
{
    SLL_Iterator<int> sut{&node1_};
    SLL_ConstIterator<int> csut = sut;
    ASSERT_EQ(*csut, *sut);
}

class SllIteratorArrowTest : public ::testing::Test {
protected:
    using Node = SLL_Node<std::string>;
    Node node1{"1"};
    Node node2{"2"};
    Node node3{"3"};

    template<typename SLL_IteratorType>
    using access_assign_using_arrow = decltype(std::declval<SLL_IteratorType&>()->assign('0'));
};

TEST_F(SllIteratorArrowTest, arrow_operator_accesses_members_of_data)
{
    SLL_Iterator<std::string> sut{&node1};
    sut->push_back('3');
    ASSERT_EQ(node1.data, *sut);
}


TEST_F(SllIteratorArrowTest, const_iterator_arrow_operator_accesses_only_const_members)
{
    SLL_ConstIterator<std::string> sut{&node1};
    auto const is_const_accessible{is_detected<access_assign_using_arrow, decltype(sut)>::value};
    ASSERT_FALSE(is_const_accessible);
}

} // namespace
