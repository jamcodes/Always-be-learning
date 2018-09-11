#include "gtest/gtest.h"
#include "Token.h"


namespace
{

class TokenTest : public ::testing::Test { };

TEST_F(TokenTest, CharLiteral)
{
    const auto lptoken = Token{std::in_place_type_t<lparen>{}};
    const auto rptoken = Token{std::in_place_type_t<rparen>{}};
    ASSERT_TRUE(std::holds_alternative<lparen>(lptoken));
    ASSERT_EQ(lparen{}, std::get<lparen>(lptoken));
    ASSERT_EQ('(', std::get<lparen>(lptoken));
    ASSERT_NE(lptoken, rptoken);
    ASSERT_EQ(lptoken, Token{std::in_place_type_t<lparen>{}});
    ASSERT_NE(rparen{}, std::get<lparen>(lptoken));
    ASSERT_NE(')', std::get<lparen>(lptoken));
}

struct test_value{ static constexpr inline auto value = std::string_view{"test"}; };

TEST_F(TokenTest, StringLiteral)
{
    const auto deftoken = Token{std::in_place_type_t<def>{}};
    ASSERT_EQ(deftoken, deftoken);

    using test_literal = token_literal<test_value>;
    using TestToken = std::variant<def, test_literal>;
    const auto test_def = TestToken{std::in_place_type_t<def>{}};
    const auto test_token = TestToken{std::in_place_type_t<test_literal>{}};
    ASSERT_NE(test_def, test_token);

    ASSERT_TRUE(std::holds_alternative<def>(deftoken));

    const auto res = std::visit([](auto&& val){
            using T = std::decay_t<decltype(val)>;
            if constexpr (std::is_same_v<T, def>) {
                return val == def::value;
            }
            return false;
        }
        ,deftoken);
    ASSERT_TRUE(res);
}

TEST_F(TokenTest, IntVariable)
{
    const auto tvar = integer{42};
    const auto test42 = Token{std::in_place_type_t<integer>{}, 42};
    const auto test11 = Token{std::in_place_type_t<integer>{}, 11};

    ASSERT_EQ(test42, test42);
    ASSERT_NE(test42, test11);
    ASSERT_LT(test11, test42);
    ASSERT_GT(test42, test11);

    ASSERT_TRUE(std::visit([](auto&& val){
            using T = std::decay_t<decltype(val)>;
            if constexpr (std::is_same_v<T,integer>) {
                return val() == 42;
            }
            return false;
        }
        ,test42));
}

TEST_F(TokenTest, StringVariable)
{
    const auto strvar = name{"foo"};
    const auto footoken = Token{std::in_place_type_t<name>{}, "foo"};
    const auto bartoken = Token{std::in_place_type_t<name>{}, "bar"};

    ASSERT_EQ(footoken, footoken);
    ASSERT_NE(footoken, bartoken);

    ASSERT_TRUE(std::visit([&s=strvar](auto&& val){
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, name>) {
            return val() == s;
        }
        return false;
    }
    ,footoken));
}

} // namespace
