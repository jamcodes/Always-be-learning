#include <gtest/gtest.h>

#include <hash_map/hash_map.hpp>

#include <array>
#include <algorithm>
#include <utility>
#include <memory>
#include <unordered_map>

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
    HashMap<int, double> sut{};
};

TEST_F(HashMapCapacityTests, empty_observer_test)
{
    ASSERT_TRUE(sut.empty());
    sut.insert({1, 1.1});
    ASSERT_FALSE(sut.empty());
}

TEST_F(HashMapCapacityTests, size_observer_test)
{
    ASSERT_EQ(sut.size(), 0);
    sut.insert({1, 1.1});
    ASSERT_EQ(sut.size(), 1);
}

class HashMapInseringTests : public ::testing::Test {
public:
    static constexpr std::array<std::pair<int, double>, 5> init{{{2, 2.2}, {3, 3.3}, {1, 1.1}, {5, 5.5}, {4, 4.4}}};

protected:
    HashMap<int, double> sut{init.cbegin(), init.cend()};

    HashMapInseringTests()
    {
        EXPECT_EQ(sut.size(), init.size());
    }
};

TEST_F(HashMapInseringTests, insert_is_noop_if_key_already_exists)
{
    auto const existing_element{init[0]};
    auto res = sut.insert(existing_element);
    ASSERT_FALSE(res.second);
    ASSERT_EQ(res.first->first, existing_element.first);
    ASSERT_EQ(res.first->second, existing_element.second);
}

TEST_F(HashMapInseringTests, insert_adds_element_if_doesnt_exist)
{
    std::pair<const int, double> const new_element{42, 42.42};
    auto res = sut.insert(new_element);
    ASSERT_TRUE(res.second);
    ASSERT_EQ(res.first->first, new_element.first);
    ASSERT_EQ(res.first->second, new_element.second);
}

TEST_F(HashMapInseringTests, insert_or_assign_assigns_value_if_element_exists)
{
    auto const existing_element{init[0]};
    std::pair<const int, double> const modified_element{existing_element.first, 42.42};
    auto res = sut.insert_or_assign(modified_element.first, modified_element.second);
    ASSERT_FALSE(res.second);
    ASSERT_EQ(res.first->first, modified_element.first);
    ASSERT_EQ(res.first->second, modified_element.second);
}

TEST_F(HashMapInseringTests, insert_or_assign_adds_element_if_doesnt_exist)
{
    std::pair<const int, double> const new_element{42, 42.42};
    auto res = sut.insert_or_assign(new_element.first, new_element.second);
    ASSERT_TRUE(res.second);
    ASSERT_EQ(res.first->first, new_element.first);
    ASSERT_EQ(res.first->second, new_element.second);
}

TEST_F(HashMapInseringTests, try_emplace_adds_element_if_doesnt_exist)
{
    constexpr int new_key{42};
    constexpr double new_value{42.42};
    auto res = sut.try_emplace(new_key, double{new_value});
    ASSERT_TRUE(res.second);
    ASSERT_EQ(res.first->first, new_key);
    ASSERT_EQ(res.first->second, new_value);
}

TEST_F(HashMapInseringTests, try_emplace_is_noop_if_element_exists)
{
    auto const existing_element{init[0]};
    std::pair<const int, double> const modified_element{existing_element.first, 42.42};
    auto res = sut.try_emplace(modified_element.first, modified_element.second);
    ASSERT_FALSE(res.second);
    ASSERT_EQ(res.first->first, existing_element.first);
    ASSERT_EQ(res.first->second, existing_element.second);
    ASSERT_EQ(res.first->first, modified_element.first);
    ASSERT_NE(res.first->second, modified_element.second);
}

class HashMapLookupTests : public ::testing::Test {
public:
    static constexpr std::array<std::pair<int, double>, 5> init{{{2, 2.2}, {3, 3.3}, {1, 1.1}, {5, 5.5}, {4, 4.4}}};

protected:
    HashMap<int, double> sut{init.cbegin(), init.cend()};

    HashMapLookupTests()
    {
        EXPECT_EQ(sut.size(), init.size());
    }
};

TEST_F(HashMapLookupTests, at_returns_a_reference_to_existing_element)
{
    auto const elem{init[0]};
    auto const& res{sut.at(elem.first)};
    ASSERT_EQ(res.first, elem.first);
    ASSERT_EQ(res.second, elem.second);
}

TEST_F(HashMapLookupTests, at_throws_if_key_doesnt_exist)
{
    std::pair<int, double> const not_elem{42, 42.42};
    ASSERT_THROW(sut.at(not_elem.first), std::out_of_range);
}

TEST_F(HashMapLookupTests, index_operator_returns_reference_to_existing_element)
{
    auto const elem{init[0]};
    auto const& res{sut[elem.first]};
    ASSERT_EQ(res.first, elem.first);
    ASSERT_EQ(res.second, elem.second);
}

TEST_F(HashMapLookupTests, index_operator_inserts_element_and_returns_reference_if_doesnt_exist)
{
    std::pair<int, double> const elem{42, 42.42};
    decltype(sut)::mapped_type value_init_mapped_type{};
    auto const& res{sut[elem.first]};
    ASSERT_EQ(res.first, elem.first);
    ASSERT_EQ(res.second, value_init_mapped_type);
}

TEST_F(HashMapLookupTests, find_returns_iterator_to_element_with_matching_key)
{
    auto const elem{init[init.size()/2]};
    auto res = sut.find(elem.first);
    ASSERT_NE(res, sut.end());
    ASSERT_EQ(res->first, elem.first);
    ASSERT_EQ(res->second, elem.second);
}

TEST_F(HashMapLookupTests, find_returns_end_iterator_if_key_doesnt_exist)
{
    std::pair<int, double> const elem{42, 42.42};
    auto res = sut.find(elem.first);
    ASSERT_EQ(res, sut.end());
}

TEST(HashMapEqualRangeTest, equal_range_returns_single_element_range)
{
    constexpr std::array<std::pair<int, double>, 6> init{
        {{2, 2.2}, {3, 3.3}, {1, 1.1}, {1, 11.11}, {5, 5.5}, {4, 4.4}}
    };
    constexpr int key{1};           // present twice in the init data
    constexpr double value{1.1};    // the element inserted first should be present
    HashMap<int, double> sut(init.cbegin(), init.cend());
    auto res = sut.equal_range(key);
    auto const count_range{std::distance(res.first, res.second)};
    ASSERT_EQ(count_range, 1);
    ASSERT_EQ(res.first->first, key);
    ASSERT_EQ(res.first->second, value);
}

class HashMapModifyingTests : public ::testing::Test {
public:
    static constexpr std::array<std::pair<int, double>, 5> init{{{2, 2.2}, {3, 3.3}, {1, 1.1}, {5, 5.5}, {4, 4.4}}};

protected:
    HashMap<int, double> sut{init.cbegin(), init.cend()};

    HashMapModifyingTests()
    {
        EXPECT_EQ(sut.size(), init.size());
    }
};

TEST_F(HashMapModifyingTests, erase_removes_element_with_equal_key)
{
    std::vector<std::pair<int, double>> reference(init.cbegin(), init.cend());
    ASSERT_TRUE(std::is_permutation(reference.cbegin(), reference.cend(), sut.cbegin(), sut.cend(),
        [](auto const& lhs, auto const& rhs)noexcept{return lhs.first == rhs.first && lhs.second == rhs.second;}));
    auto const to_erase{init[init.size()/2]};
    auto const erased_count{sut.erase(to_erase.first)};
    reference.erase(std::remove_if(reference.begin(), reference.end(), [&key=to_erase.first](auto const& e)noexcept{
        return e.first == key;
    }), reference.end());
    ASSERT_EQ(erased_count, init.size() - reference.size());
    ASSERT_TRUE(std::is_permutation(reference.cbegin(), reference.cend(), sut.cbegin(), sut.cend(),
        [](auto const& lhs, auto const& rhs)noexcept{return lhs.first == rhs.first && lhs.second == rhs.second;}));
}

TEST_F(HashMapModifyingTests, erase_removes_element_at_position)
{
    std::vector<std::pair<int, double>> reference(init.cbegin(), init.cend());
    ASSERT_TRUE(std::is_permutation(reference.cbegin(), reference.cend(), sut.cbegin(), sut.cend(),
        [](auto const& lhs, auto const& rhs)noexcept{return lhs.first == rhs.first && lhs.second == rhs.second;}));
    auto const elem{reference[reference.size()/2]};
    // auto const it_erase{sut.find(elem.first)};
    auto it_erase{sut.cbegin()};
    while (it_erase != sut.cend() && it_erase->first != elem.first) {
        ++it_erase;
    }
    ASSERT_NE(it_erase, sut.cend());
    auto const res{sut.erase(it_erase)};
    reference.erase(std::remove_if(reference.begin(), reference.end(), [&key=elem.first](auto const& e)noexcept{
        return e.first == key;
    }));
    ASSERT_TRUE(std::is_permutation(reference.cbegin(), reference.cend(), sut.cbegin(), sut.cend(),
    [](auto const& lhs, auto const& rhs)noexcept{ return lhs.first == rhs.first && lhs.second == rhs.second; }));
}

TEST_F(HashMapModifyingTests, rehash_changes_capacity_and_load_factor_keeps_elements)
{
    std::size_t constexpr new_capacity{97};
    auto const init_load_factor{sut.load_factor()};
    sut.rehash(new_capacity);
    auto const new_load_factor{sut.load_factor()};
    ASSERT_LT(new_load_factor, init_load_factor);
    ASSERT_TRUE(std::is_permutation(sut.cbegin(), sut.cend(), init.cbegin(), init.cend(),
        [](auto const& lhs, auto const& rhs)noexcept{ return lhs.first == rhs.first && lhs.second == rhs.second; })
    );
}

} // namespace
