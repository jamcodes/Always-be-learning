#include <gtest/gtest.h>

#include <binary_tree/binary_tree.hpp>

#include <algorithm>
#include <array>
#include <utility>

namespace
{

class BinaryTreeIteratorTest : public ::testing::Test {
protected:
    constexpr static std::array<std::pair<int, double>, 5> init_{{{5, 5.5}, {2, 2.2}, {3, 3.3}, {1, 1.1}, {4, 4.4}}};
    BinaryTree<int, double> bt_{init_.cbegin(), init_.cend()};

    BinaryTreeIteratorTest()
    {
        EXPECT_FALSE(bt_.empty());
    }
};

TEST_F(BinaryTreeIteratorTest, begin_returns_inorder_iterator)
{
    const std::array<std::pair<int, double>, init_.size()> expected{
        [this](){
            std::array<std::pair<int, double>, init_.size()> res{init_};
            std::sort(res.begin(), res.end());
            return res;
        }()
    };
    auto sut{bt_.begin()};
    std::size_t count{0};
    for (auto it{expected.begin()}; it != expected.end() && sut != bt_.end(); ++it, ++sut) {
        ASSERT_EQ(sut->first, it->first);
        ASSERT_EQ(sut->second, it->second);
        ++count;
    }
    ASSERT_EQ(count, expected.size());
}

TEST_F(BinaryTreeIteratorTest, rbegin_returns_postorder_iterator)
{
    const std::array<std::pair<int, double>, init_.size()> expected{
        [this](){
            std::array<std::pair<int, double>, init_.size()> res{init_};
            std::sort(res.begin(), res.end(), [](auto const& lhs, auto const& rhs) noexcept { return rhs < lhs; });
            return res;
        }()
    };
    auto sut{bt_.rbegin()};
    ASSERT_NE(sut, bt_.rend());
    std::size_t count{0};
    for (auto it{expected.begin()}; it != expected.end() && sut != bt_.rend(); ++it, ++sut) {
        ASSERT_EQ(sut->first, it->first);
        ASSERT_EQ(sut->second, it->second);
        ++count;
    }
    ASSERT_EQ(count, expected.size());
}

} // namespace
