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

TEST_F(ParserShi, SimpleExample) {
  const String input =
      "configs = []\n"
      "\n"
      "executable(\"sample\") {\n"
      "    sources = [ \"source1\", \"source2\", ]\n"
      "    deps = [ \"library\" ]\n"
      "    if (os == \"macos\") {\n"
      "        configs += generate_smth()\n"
      "    } else if (os != \"linux\") {\n"
      "        headers -= \"test.h\" + !public_headers\n"
      "    } else {\n"
      "        testing = true && ( os == \"win\")\n"
      "    }\n"
      "}\n";

  Parse(input);
  ASSERT_TRUE(top_node->asStatementList());

  auto stmt_it = top_node->asStatementList()->begin(),
       end = top_node->asStatementList()->end();

  {
    ASSERT_NE(end, stmt_it);

    auto* stmt = (*stmt_it)->asAssignment();
    ASSERT_TRUE(stmt);

    auto* lvalue = stmt->left_value()->asIdentifier();
    ASSERT_TRUE(lvalue);
    EXPECT_EQ("configs", lvalue->identifier().value());
  }
}

}  // namespace language::shi
}  // namespace shinobi
