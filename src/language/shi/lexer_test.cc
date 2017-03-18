#include <language/shi/lexer.hh>

// Third-party
#include <gflags/gflags.h>
#include <gtest/gtest.h>

namespace shinobi {

DECLARE_string(data);

namespace language::shi {

TEST(LexerShi, AllTokens) {
  const String input =
      "1 2 3 4 5 6 7 8 9 0 12345 - = + q w e r t y u i qwertyui [ ] { }"
      " \"string\" || < , > . <= == >= += -= true false if else ! && * ( )";

  Lexer lexer("/fake/path/file.shi", input);
  auto tokens = lexer.Tokenize();

  // TODO: check tokens.

  ASSERT_EQ(47u, tokens.size());
  EXPECT_EQ(std::make_tuple("1", Token::INTEGER),
            std::make_tuple(tokens[0].value(), tokens[0].type()));
}

}  // namespace language::shi
}  // namespace shinobi
