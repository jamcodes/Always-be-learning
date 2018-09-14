#include <iostream>
#include <sstream>
#include <variant>
#include "gtest/gtest.h"
#include "Scanner.h"


namespace
{

class ScannerTest : public ::testing::Test {
protected:
    static const inline name name_foo{"foo"};
    static const inline name name_bar{"bar"};
    static const inline integer val_42{42};
    static const inline integer val_11{11};
 };


TEST_F(ScannerTest, token_char_test)
{
    std::stringstream test_src;
    test_src << lparen::value;
    test_src << rparen::value;
    test_src << plus::value;
    test_src << minus::value;
    test_src << colon::value;
    test_src << comma::value;
    test_src << equals::value;
    test_src.seekg(0);

    Scanner tstream{test_src};
    auto tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<lparen>(tt));
    tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<rparen>(tt));
    tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<plus>(tt));
    tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<minus>(tt));
    tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<colon>(tt));
    tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<comma>(tt));
    tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<equals>(tt));
    tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<eof_token>(tt));
}

TEST_F(ScannerTest, token_literal_test)
{
    std::stringstream test_src;
    test_src << def::value;
    test_src.seekg(0);

    Scanner tstream{test_src};
    auto tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<def>(tt));
}

TEST_F(ScannerTest, token_variable_test)
{
    std::stringstream test_src;
    test_src << name_foo();
    test_src << ' ';
    test_src << val_42();
    test_src << ' ';
    test_src << name_bar();
    test_src << ' ';
    test_src << val_11();
    test_src.seekg(0);

    Scanner tstream{test_src};
    auto tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<name>(tt));
    ASSERT_EQ(name_foo, std::get<name>(tt));

    tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<integer>(tt));
    ASSERT_EQ(val_42, std::get<integer>(tt));

    tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<name>(tt));
    ASSERT_EQ(name_bar, std::get<name>(tt));

    tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<integer>(tt));
    ASSERT_EQ(val_11, std::get<integer>(tt));
}

TEST_F(ScannerTest, peek_test)
{
    std::stringstream test_src;
    test_src << lparen::value;
    test_src << name_bar();
    test_src << ' ';
    test_src << val_42();
    // test_src << rparen::value;
    // test_src << plus::value;
    test_src.seekg(0);

    Scanner tstream{test_src};
    auto tt = tstream.peek();
    ASSERT_TRUE(std::holds_alternative<lparen>(tt));
#if !defined(NDEBUG)
    ASSERT_ANY_THROW(tstream.putback(tt));
#endif // NDEBUG
    auto tt2 = tstream.get();
    ASSERT_TRUE(std::holds_alternative<lparen>(tt2));
    tt = tstream.peek();
    ASSERT_TRUE(std::holds_alternative<name>(tt));
    ASSERT_EQ(name_bar, std::get<name>(tt));
    tstream.ignore(tt);
    tt2 = tstream.get();
    ASSERT_TRUE(std::holds_alternative<integer>(tt2));
    ASSERT_EQ(val_42, std::get<integer>(tt2));
}

TEST_F(ScannerTest, putback_test)
{
    std::stringstream test_src;
    test_src << lparen::value;
    test_src << rparen::value;
    test_src << name_bar();
    test_src.seekg(0);

    Scanner tstream{test_src};
    auto tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<lparen>(tt));
    tstream.putback(tt);
#if !defined(NDEBUG)
    ASSERT_ANY_THROW(tstream.putback(tt));
#endif // NDEBUG
    auto tt2 = tstream.get();
    ASSERT_TRUE(std::holds_alternative<lparen>(tt2));
    tt = tstream.get();
    tstream.putback(tt);
    tt2 = tstream.get();
    ASSERT_TRUE(std::holds_alternative<rparen>(tt2));
    tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<name>(tt));
    ASSERT_EQ(name_bar, std::get<name>(tt));
    tstream.putback(tt);
    tt2 = tstream.get();
    ASSERT_TRUE(std::holds_alternative<name>(tt2));
    ASSERT_EQ(name_bar, std::get<name>(tt));
}

TEST_F(ScannerTest, ignore_test)
{
    std::stringstream test_src;
    test_src << lparen::value;
    test_src << rparen::value;
    test_src << plus::value;
    test_src << name_foo();
    test_src.seekg(0);

    Scanner tstream{test_src};
    auto tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<lparen>(tt));
    tstream.putback(tt);
    tstream.ignore(tt);
    tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<rparen>(tt));
    tstream.ignore(Token{kind<plus>{}});
    tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<name>(tt));
    ASSERT_EQ(name_foo, std::get<name>(tt));
}

TEST_F(ScannerTest, ignore_kind_test)
{
    std::stringstream test_src;
    test_src << rparen::value;
    test_src << val_42();
    test_src << ' ';
    test_src << name_bar();
    test_src << ' ';
    test_src << def::value;
    test_src << rparen::value;
    test_src << val_42();
    test_src << name_bar();
    test_src.seekg(0);

    Scanner tstream{test_src};
    tstream.ignore(def{});
    auto tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<rparen>(tt));
    tstream.ignore(val_42);
    tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<name>(tt));
    ASSERT_EQ(name_bar, std::get<name>(tt));
}

TEST_F(ScannerTest, get_indent)
{
    const std::string_view four_space{"    "};
    std::stringstream test_src;
    test_src << four_space;     // ignore
    test_src << '\n';           // read, discard, check for indent
    test_src << four_space;     // return indent token
    test_src << def::value;     // return def token
    test_src << four_space;     // ignore, return eof token
    test_src.seekg(0);

    Scanner tstream{test_src};
    auto tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<indent>(tt));
    ASSERT_EQ(four_space.size(), std::get<indent>(tt));
    tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<def>(tt));
    tt = tstream.get();
    ASSERT_TRUE(std::holds_alternative<eof_token>(tt));
}

} // namespace
