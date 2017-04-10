#include <language/shi/lexer.hh>
#include <language/shi/parser.hh>

// Third-party
#include <gflags/gflags.h>
#include <gtest/gtest.h>

namespace shinobi {

DECLARE_string(data);

namespace language::shi {

class ParserShi : public ::testing::Test {
 protected:
  void Parse(const String& input) {
    Lexer lexer("/fake/path/file.shi", input);
    tokens = lexer.Tokenize();
    Parser parser(tokens.begin(), tokens.end());
    top_node = parser.Parse();
  }

  Vector<Token> tokens;
  NodePtr top_node;
};

}  // namespace language::shi
}  // namespace shinobi
