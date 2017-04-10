#include <language/shi/exception.hh>
#include <language/shi/lexer.hh>

// Third-party
#include <gflags/gflags.h>
#include <gtest/gtest.h>

namespace shinobi {

DECLARE_string(data);

namespace language::shi {

class LexerShi : public ::testing::Test {
 protected:
  void Tokenize(const String& input) {
    Lexer lexer("/fake/path/file.shi", input);
    tokens = lexer.Tokenize();
  }

  Vector<Token> tokens;
};

TEST_F(LexerShi, AllTokens) {
  const String input =
      "1 2 3 4 5 6 7 8 9 0 12345 - = + q w e r t y u i qwertyui [ ] { }"
      " \"string\" || < , > . <= == >= += -= true false if else ! && ( )";
  Tokenize(input);

  ASSERT_EQ(47u, tokens.size());
  EXPECT_EQ(std::make_tuple("1", Token::INTEGER),
            std::make_tuple(tokens[0].value(), tokens[0].type()));
  EXPECT_EQ(std::make_tuple("2", Token::INTEGER),
            std::make_tuple(tokens[1].value(), tokens[1].type()));
  EXPECT_EQ(std::make_tuple("3", Token::INTEGER),
            std::make_tuple(tokens[2].value(), tokens[2].type()));
  EXPECT_EQ(std::make_tuple("4", Token::INTEGER),
            std::make_tuple(tokens[3].value(), tokens[3].type()));
  EXPECT_EQ(std::make_tuple("5", Token::INTEGER),
            std::make_tuple(tokens[4].value(), tokens[4].type()));
  EXPECT_EQ(std::make_tuple("6", Token::INTEGER),
            std::make_tuple(tokens[5].value(), tokens[5].type()));
  EXPECT_EQ(std::make_tuple("7", Token::INTEGER),
            std::make_tuple(tokens[6].value(), tokens[6].type()));
  EXPECT_EQ(std::make_tuple("8", Token::INTEGER),
            std::make_tuple(tokens[7].value(), tokens[7].type()));
  EXPECT_EQ(std::make_tuple("9", Token::INTEGER),
            std::make_tuple(tokens[8].value(), tokens[8].type()));
  EXPECT_EQ(std::make_tuple("0", Token::INTEGER),
            std::make_tuple(tokens[9].value(), tokens[9].type()));
  EXPECT_EQ(std::make_tuple("12345", Token::INTEGER),
            std::make_tuple(tokens[10].value(), tokens[10].type()));
  EXPECT_EQ(std::make_tuple("-", Token::MINUS),
            std::make_tuple(tokens[11].value(), tokens[11].type()));
  EXPECT_EQ(std::make_tuple("=", Token::EQUAL),
            std::make_tuple(tokens[12].value(), tokens[12].type()));
  EXPECT_EQ(std::make_tuple("+", Token::PLUS),
            std::make_tuple(tokens[13].value(), tokens[13].type()));
  EXPECT_EQ(std::make_tuple("q", Token::IDENTIFIER),
            std::make_tuple(tokens[14].value(), tokens[14].type()));
  EXPECT_EQ(std::make_tuple("w", Token::IDENTIFIER),
            std::make_tuple(tokens[15].value(), tokens[15].type()));
  EXPECT_EQ(std::make_tuple("e", Token::IDENTIFIER),
            std::make_tuple(tokens[16].value(), tokens[16].type()));
  EXPECT_EQ(std::make_tuple("r", Token::IDENTIFIER),
            std::make_tuple(tokens[17].value(), tokens[17].type()));
  EXPECT_EQ(std::make_tuple("t", Token::IDENTIFIER),
            std::make_tuple(tokens[18].value(), tokens[18].type()));
  EXPECT_EQ(std::make_tuple("y", Token::IDENTIFIER),
            std::make_tuple(tokens[19].value(), tokens[19].type()));
  EXPECT_EQ(std::make_tuple("u", Token::IDENTIFIER),
            std::make_tuple(tokens[20].value(), tokens[20].type()));
  EXPECT_EQ(std::make_tuple("i", Token::IDENTIFIER),
            std::make_tuple(tokens[21].value(), tokens[21].type()));
  EXPECT_EQ(std::make_tuple("qwertyui", Token::IDENTIFIER),
            std::make_tuple(tokens[22].value(), tokens[22].type()));
  EXPECT_EQ(std::make_tuple("[", Token::LEFT_BRACKET),
            std::make_tuple(tokens[23].value(), tokens[23].type()));
  EXPECT_EQ(std::make_tuple("]", Token::RIGHT_BRACKET),
            std::make_tuple(tokens[24].value(), tokens[24].type()));
  EXPECT_EQ(std::make_tuple("{", Token::LEFT_BRACE),
            std::make_tuple(tokens[25].value(), tokens[25].type()));
  EXPECT_EQ(std::make_tuple("}", Token::RIGHT_BRACE),
            std::make_tuple(tokens[26].value(), tokens[26].type()));
  EXPECT_EQ(std::make_tuple("\"string\"", Token::STRING),
            std::make_tuple(tokens[27].value(), tokens[27].type()));
  EXPECT_EQ(std::make_tuple("||", Token::BOOLEAN_OR),
            std::make_tuple(tokens[28].value(), tokens[28].type()));
  EXPECT_EQ(std::make_tuple("<", Token::STRICTLY_LESS),
            std::make_tuple(tokens[29].value(), tokens[29].type()));
  EXPECT_EQ(std::make_tuple(",", Token::COMMA),
            std::make_tuple(tokens[30].value(), tokens[30].type()));
  EXPECT_EQ(std::make_tuple(">", Token::STRICTLY_GREATER),
            std::make_tuple(tokens[31].value(), tokens[31].type()));
  EXPECT_EQ(std::make_tuple(".", Token::DOT),
            std::make_tuple(tokens[32].value(), tokens[32].type()));
  EXPECT_EQ(std::make_tuple("<=", Token::LESS_EQUAL),
            std::make_tuple(tokens[33].value(), tokens[33].type()));
  EXPECT_EQ(std::make_tuple("==", Token::EQUAL_EQUAL),
            std::make_tuple(tokens[34].value(), tokens[34].type()));
  EXPECT_EQ(std::make_tuple(">=", Token::GREATER_EQUAL),
            std::make_tuple(tokens[35].value(), tokens[35].type()));
  EXPECT_EQ(std::make_tuple("+=", Token::PLUS_EQUALS),
            std::make_tuple(tokens[36].value(), tokens[36].type()));
  EXPECT_EQ(std::make_tuple("-=", Token::MINUS_EQUALS),
            std::make_tuple(tokens[37].value(), tokens[37].type()));
  EXPECT_EQ(std::make_tuple("true", Token::TRUE_TOKEN),
            std::make_tuple(tokens[38].value(), tokens[38].type()));
  EXPECT_EQ(std::make_tuple("false", Token::FALSE_TOKEN),
            std::make_tuple(tokens[39].value(), tokens[39].type()));
  EXPECT_EQ(std::make_tuple("if", Token::IF_TOKEN),
            std::make_tuple(tokens[40].value(), tokens[40].type()));
  EXPECT_EQ(std::make_tuple("else", Token::ELSE_TOKEN),
            std::make_tuple(tokens[41].value(), tokens[41].type()));
  EXPECT_EQ(std::make_tuple("!", Token::BANG),
            std::make_tuple(tokens[42].value(), tokens[42].type()));
  EXPECT_EQ(std::make_tuple("&&", Token::BOOLEAN_AND),
            std::make_tuple(tokens[43].value(), tokens[43].type()));
  EXPECT_EQ(std::make_tuple("(", Token::LEFT_PAREN),
            std::make_tuple(tokens[44].value(), tokens[44].type()));
  EXPECT_EQ(std::make_tuple(")", Token::RIGHT_PAREN),
            std::make_tuple(tokens[45].value(), tokens[45].type()));
  EXPECT_EQ(Token::INVALID, tokens[46].type());

  // TODO: check all token locations and ranges.
}

TEST_F(LexerShi, Whitespace) {
  const String input = " \t\n \t1\n2 \t3 \t4\n \t\n";
  Tokenize(input);

  ASSERT_EQ(5u, tokens.size());
  EXPECT_EQ(std::make_tuple("1", Token::INTEGER),
            std::make_tuple(tokens[0].value(), tokens[0].type()));
  EXPECT_EQ(std::make_tuple("2", Token::INTEGER),
            std::make_tuple(tokens[1].value(), tokens[1].type()));
  EXPECT_EQ(std::make_tuple("3", Token::INTEGER),
            std::make_tuple(tokens[2].value(), tokens[2].type()));
  EXPECT_EQ(std::make_tuple("4", Token::INTEGER),
            std::make_tuple(tokens[3].value(), tokens[3].type()));
  EXPECT_EQ(Token::INVALID, tokens[4].type());
}

TEST_F(LexerShi, Comments) {
  const String input =
      "#\n"
      " \t#\n"
      " \t# \t\n"
      "1 # 1\n"
      "2#\n"
      "#\"\n"
      "#1#";
  Tokenize(input);

  ASSERT_EQ(10u, tokens.size());
  EXPECT_EQ(Token::COMMENT, tokens[0].type());
  EXPECT_EQ(Token::COMMENT, tokens[1].type());
  EXPECT_EQ(Token::COMMENT, tokens[2].type());
  EXPECT_EQ(std::make_tuple("1", Token::INTEGER),
            std::make_tuple(tokens[3].value(), tokens[3].type()));
  EXPECT_EQ(Token::COMMENT, tokens[4].type());
  EXPECT_EQ(std::make_tuple("2", Token::INTEGER),
            std::make_tuple(tokens[5].value(), tokens[5].type()));
  EXPECT_EQ(Token::COMMENT, tokens[6].type());
  EXPECT_EQ(Token::COMMENT, tokens[7].type());
  EXPECT_EQ(Token::COMMENT, tokens[8].type());
  EXPECT_EQ(Token::INVALID, tokens[9].type());
}

TEST_F(LexerShi, Identifiers) {
  const String input = "q1w2e3 ifq if1 ifelse";
  Tokenize(input);

  ASSERT_EQ(5u, tokens.size());
  EXPECT_EQ(std::make_tuple("q1w2e3", Token::IDENTIFIER),
            std::make_tuple(tokens[0].value(), tokens[0].type()));
  EXPECT_EQ(std::make_tuple("ifq", Token::IDENTIFIER),
            std::make_tuple(tokens[1].value(), tokens[1].type()));
  EXPECT_EQ(std::make_tuple("if1", Token::IDENTIFIER),
            std::make_tuple(tokens[2].value(), tokens[2].type()));
  EXPECT_EQ(std::make_tuple("ifelse", Token::IDENTIFIER),
            std::make_tuple(tokens[3].value(), tokens[3].type()));
  EXPECT_EQ(Token::INVALID, tokens[4].type());
}

TEST_F(LexerShi, NoWhitespace) {
  const String input =
      "1-2=3+4qwerty[]{}\"string\"||<,>.<===>=+=-=!&&()if+else+true+false";
  Tokenize(input);

  ASSERT_EQ(35u, tokens.size());
  EXPECT_EQ(std::make_tuple("1", Token::INTEGER),
            std::make_tuple(tokens[0].value(), tokens[0].type()));
  EXPECT_EQ(std::make_tuple("-", Token::MINUS),
            std::make_tuple(tokens[1].value(), tokens[1].type()));
  EXPECT_EQ(std::make_tuple("2", Token::INTEGER),
            std::make_tuple(tokens[2].value(), tokens[2].type()));
  EXPECT_EQ(std::make_tuple("=", Token::EQUAL),
            std::make_tuple(tokens[3].value(), tokens[3].type()));
  EXPECT_EQ(std::make_tuple("3", Token::INTEGER),
            std::make_tuple(tokens[4].value(), tokens[4].type()));
  EXPECT_EQ(std::make_tuple("+", Token::PLUS),
            std::make_tuple(tokens[5].value(), tokens[5].type()));
  EXPECT_EQ(std::make_tuple("4", Token::INTEGER),
            std::make_tuple(tokens[6].value(), tokens[6].type()));
  EXPECT_EQ(std::make_tuple("qwerty", Token::IDENTIFIER),
            std::make_tuple(tokens[7].value(), tokens[7].type()));
  EXPECT_EQ(std::make_tuple("[", Token::LEFT_BRACKET),
            std::make_tuple(tokens[8].value(), tokens[8].type()));
  EXPECT_EQ(std::make_tuple("]", Token::RIGHT_BRACKET),
            std::make_tuple(tokens[9].value(), tokens[9].type()));
  EXPECT_EQ(std::make_tuple("{", Token::LEFT_BRACE),
            std::make_tuple(tokens[10].value(), tokens[10].type()));
  EXPECT_EQ(std::make_tuple("}", Token::RIGHT_BRACE),
            std::make_tuple(tokens[11].value(), tokens[11].type()));
  EXPECT_EQ(std::make_tuple("\"string\"", Token::STRING),
            std::make_tuple(tokens[12].value(), tokens[12].type()));
  EXPECT_EQ(std::make_tuple("||", Token::BOOLEAN_OR),
            std::make_tuple(tokens[13].value(), tokens[13].type()));
  EXPECT_EQ(std::make_tuple("<", Token::STRICTLY_LESS),
            std::make_tuple(tokens[14].value(), tokens[14].type()));
  EXPECT_EQ(std::make_tuple(",", Token::COMMA),
            std::make_tuple(tokens[15].value(), tokens[15].type()));
  EXPECT_EQ(std::make_tuple(">", Token::STRICTLY_GREATER),
            std::make_tuple(tokens[16].value(), tokens[16].type()));
  EXPECT_EQ(std::make_tuple(".", Token::DOT),
            std::make_tuple(tokens[17].value(), tokens[17].type()));
  EXPECT_EQ(std::make_tuple("<=", Token::LESS_EQUAL),
            std::make_tuple(tokens[18].value(), tokens[18].type()));
  EXPECT_EQ(std::make_tuple("==", Token::EQUAL_EQUAL),
            std::make_tuple(tokens[19].value(), tokens[19].type()));
  EXPECT_EQ(std::make_tuple(">=", Token::GREATER_EQUAL),
            std::make_tuple(tokens[20].value(), tokens[20].type()));
  EXPECT_EQ(std::make_tuple("+=", Token::PLUS_EQUALS),
            std::make_tuple(tokens[21].value(), tokens[21].type()));
  EXPECT_EQ(std::make_tuple("-=", Token::MINUS_EQUALS),
            std::make_tuple(tokens[22].value(), tokens[22].type()));
  EXPECT_EQ(std::make_tuple("!", Token::BANG),
            std::make_tuple(tokens[23].value(), tokens[23].type()));
  EXPECT_EQ(std::make_tuple("&&", Token::BOOLEAN_AND),
            std::make_tuple(tokens[24].value(), tokens[24].type()));
  EXPECT_EQ(std::make_tuple("(", Token::LEFT_PAREN),
            std::make_tuple(tokens[25].value(), tokens[25].type()));
  EXPECT_EQ(std::make_tuple(")", Token::RIGHT_PAREN),
            std::make_tuple(tokens[26].value(), tokens[26].type()));
  EXPECT_EQ(std::make_tuple("if", Token::IF_TOKEN),
            std::make_tuple(tokens[27].value(), tokens[27].type()));
  EXPECT_EQ(std::make_tuple("+", Token::PLUS),
            std::make_tuple(tokens[28].value(), tokens[28].type()));
  EXPECT_EQ(std::make_tuple("else", Token::ELSE_TOKEN),
            std::make_tuple(tokens[29].value(), tokens[29].type()));
  EXPECT_EQ(std::make_tuple("+", Token::PLUS),
            std::make_tuple(tokens[30].value(), tokens[30].type()));
  EXPECT_EQ(std::make_tuple("true", Token::TRUE_TOKEN),
            std::make_tuple(tokens[31].value(), tokens[31].type()));
  EXPECT_EQ(std::make_tuple("+", Token::PLUS),
            std::make_tuple(tokens[32].value(), tokens[32].type()));
  EXPECT_EQ(std::make_tuple("false", Token::FALSE_TOKEN),
            std::make_tuple(tokens[33].value(), tokens[33].type()));
  EXPECT_EQ(Token::INVALID, tokens[34].type());
}

TEST_F(LexerShi, GoodStrings) {
  const String input =
      "\"1#*1\""
      "\"\"\n";
  Tokenize(input);

  ASSERT_EQ(3u, tokens.size());

  // Test dash (potential comment) inside string and unknown symbol.
  EXPECT_EQ(std::make_tuple("\"1#*1\"", Token::STRING),
            std::make_tuple(tokens[0].value(), tokens[0].type()));

  // Test empty string and two strings in-a-row.
  EXPECT_EQ(std::make_tuple("\"\"", Token::STRING),
            std::make_tuple(tokens[1].value(), tokens[1].type()));
  EXPECT_EQ(Token::INVALID, tokens[2].type());
}

TEST_F(LexerShi, BadStrings) {
  EXPECT_THROW({ Tokenize("\"123\n\""); }, SyntaxError);
  EXPECT_THROW({ Tokenize("\"123"); }, SyntaxError);
}

TEST_F(LexerShi, UnknownSymbol) {
  EXPECT_THROW({ Tokenize("*"); }, SyntaxError);
}

}  // namespace language::shi
}  // namespace shinobi
