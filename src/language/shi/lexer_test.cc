#include <language/shi/lexer.hh>

// Third-party
#include <gflags/gflags.h>
#include <gtest/gtest.h>

namespace shinobi {

DECLARE_string(data);

namespace language::shi {

TEST(LexerShi, DISABLED_AllTokens) {
  const String input =
      "1 2 3 4 5 6 7 8 9 0 12345 - = + q w e r t y u i qwertyui [ ] { } \" || "
      "< , > . <= == >= += -= true false if else ! && * ( )";

  Lexer lexer("/fake/path/file.shi", input);
  auto tokens = lexer.Tokenize();

  // TODO: check tokens.
}

}  // namespace language::shi
}  // namespace shinobi
