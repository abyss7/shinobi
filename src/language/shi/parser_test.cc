#include <language/shi/exception.hh>
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

TEST_F(ParserShi, EmptyTopNode) {
  Parse("");

  ASSERT_EQ(Node::STATEMENT_LIST, top_node->type());
  auto stmt_it = top_node->asStatementList()->begin(),
       stmt_end = top_node->asStatementList()->end();
  ASSERT_EQ(stmt_end, stmt_it);
}

TEST_F(ParserShi, AssignmentNoLeftValue) {
  const String input = "= b";

  EXPECT_THROW({ Parse(input); }, UnexpectedToken);
}

TEST_F(ParserShi, AssignmentNoRightValue) {
  const String input = "a = ";

  EXPECT_THROW({ Parse(input); }, SemanticError);
}

TEST_F(ParserShi, ComplexExample) {
  const String input =
      "configs = []\n"
      "\n"
      "executable(\"sample\", 123) {\n"
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

  auto CheckIdentifier = [](const Node* node, const String& name) {
    ASSERT_EQ(Node::IDENTIFIER, node->type());
    EXPECT_EQ(name, node->asIdentifier()->identifier().value());
  };

  auto CheckLiteral = [](const NodePtr& node, Token::Type type,
                         const String& value) {
    ASSERT_EQ(Node::LITERAL, node->type());
    auto* literal = node->asLiteral();

    EXPECT_EQ(type, literal->value().type());
    EXPECT_EQ(value, literal->value().value());
  };

  ASSERT_EQ(Node::STATEMENT_LIST, top_node->type());
  auto stmt_it = top_node->asStatementList()->begin(),
       stmt_end = top_node->asStatementList()->end();
  ASSERT_NE(stmt_end, stmt_it);

  // configs = []
  ASSERT_EQ(Node::ASSIGNMENT, (*stmt_it)->type());
  {
    auto* stmt = (*stmt_it)->asAssignment();

    // "configs"
    CheckIdentifier(stmt->left_value(), "configs");

    // "="
    EXPECT_EQ(Token::EQUAL, stmt->operation().type());

    // "[]"
    ASSERT_EQ(Node::EXPRESSION_LIST, stmt->right_value()->type());
    auto* expr_list = stmt->right_value()->asExpressionList();
    EXPECT_EQ(expr_list->end(), expr_list->begin());
  }

  ASSERT_NE(stmt_end, ++stmt_it);

  // executable("sample", 123)
  ASSERT_EQ(Node::CALL, (*stmt_it)->type());
  {
    auto* call = (*stmt_it)->asCall();

    // "executable"
    EXPECT_EQ(Token::IDENTIFIER, call->identifier().type());
    EXPECT_EQ("executable", call->identifier().value());

    // ("sample", 123)
    ASSERT_EQ(Node::EXPRESSION_LIST, call->expression_list()->type());
    auto expr_it = call->expression_list()->asExpressionList()->begin(),
         expr_end = call->expression_list()->asExpressionList()->end();
    ASSERT_NE(expr_end, expr_it);

    // "sample"
    CheckLiteral(*expr_it, Token::STRING, "\"sample\"");

    ASSERT_NE(expr_end, ++expr_it);

    // 123
    CheckLiteral(*expr_it, Token::INTEGER, "123");

    ASSERT_EQ(expr_end, ++expr_it);
  }

  // { … }
  ASSERT_TRUE((*stmt_it)->asCall()->block());
  ASSERT_EQ(Node::STATEMENT_LIST, (*stmt_it)->asCall()->block()->type());
  {
    auto* block = (*stmt_it)->asCall()->block();
    auto block_it = block->asStatementList()->begin(),
         block_end = block->asStatementList()->end();
    ASSERT_NE(block_end, block_it);

    // sources = [ "source1", "source2", ]
    ASSERT_EQ(Node::ASSIGNMENT, (*block_it)->type());
    {
      auto* stmt = (*block_it)->asAssignment();

      // "sources"
      CheckIdentifier(stmt->left_value(), "sources");

      // "="
      EXPECT_EQ(Token::EQUAL, stmt->operation().type());

      // "[ "source1", "source2", ]"
      ASSERT_EQ(Node::EXPRESSION_LIST, stmt->right_value()->type());
      auto* expr_list = stmt->right_value()->asExpressionList();
      auto expr_it = expr_list->asExpressionList()->begin(),
           expr_end = expr_list->asExpressionList()->end();
      ASSERT_NE(expr_end, expr_it);

      // "source1"
      CheckLiteral(*expr_it, Token::STRING, "\"source1\"");

      ASSERT_NE(expr_end, ++expr_it);

      // "source2"
      CheckLiteral(*expr_it, Token::STRING, "\"source2\"");

      ASSERT_EQ(expr_end, ++expr_it);
    }

    ASSERT_NE(block_end, ++block_it);

    // deps = [ "library" ]
    ASSERT_EQ(Node::ASSIGNMENT, (*block_it)->type());
    {
      auto* stmt = (*block_it)->asAssignment();

      // "deps"
      CheckIdentifier(stmt->left_value(), "deps");

      // "="
      EXPECT_EQ(Token::EQUAL, stmt->operation().type());

      // "[ "library" ]"
      ASSERT_EQ(Node::EXPRESSION_LIST, stmt->right_value()->type());
      auto* expr_list = stmt->right_value()->asExpressionList();
      auto expr_it = expr_list->asExpressionList()->begin(),
           expr_end = expr_list->asExpressionList()->end();
      ASSERT_NE(expr_end, expr_it);

      // "library"
      CheckLiteral(*expr_it, Token::STRING, "\"library\"");

      ASSERT_EQ(expr_end, ++expr_it);
    }

    ASSERT_NE(block_end, ++block_it);

    // if (os == "macos") { … }
    ASSERT_EQ(Node::CONDITION, (*block_it)->type());
    {
      auto* cond = (*block_it)->asCondition();

      // os == "macos"
      ASSERT_EQ(Node::BINARY_OP, cond->if_expression()->type());
      {
        auto* op = cond->if_expression()->asBinaryOp();

        // "=="
        ASSERT_EQ(Token::EQUAL_EQUAL, op->operation().type());

        // "os"
        CheckIdentifier(op->left_expression(), "os");

        // "macos"
        ASSERT_EQ(Node::LITERAL, op->right_expression()->type());
        {
          auto* right = op->right_expression()->asLiteral();

          ASSERT_EQ(Token::STRING, right->value().type());
          EXPECT_EQ("\"macos\"", right->value().value());
        }
      }

      // { … }
      ASSERT_EQ(Node::STATEMENT_LIST, cond->if_block()->type());
      {
        auto if_block_it = cond->if_block()->asStatementList()->begin(),
             if_block_end = cond->if_block()->asStatementList()->end();
        ASSERT_NE(if_block_end, if_block_it);

        // configs += generate_smth()
        ASSERT_EQ(Node::ASSIGNMENT, (*if_block_it)->type());
        {
          auto* assignment = (*if_block_it)->asAssignment();

          // "configs"
          CheckIdentifier(assignment->left_value(), "configs");

          // "+="
          EXPECT_EQ(Token::PLUS_EQUALS, assignment->operation().type());

          // generate_smth()
          ASSERT_EQ(Node::CALL, assignment->right_value()->type());
          {
            auto* call = assignment->right_value()->asCall();

            // "generate_smth"
            ASSERT_EQ(Token::IDENTIFIER, call->identifier().type());
            EXPECT_EQ("generate_smth", call->identifier().value());

            // "()"
            ASSERT_EQ(Node::EXPRESSION_LIST, call->expression_list()->type());
            auto* expr_list = call->expression_list()->asExpressionList();
            EXPECT_EQ(expr_list->end(), expr_list->begin());

            EXPECT_FALSE(call->block());
          }
        }

        ASSERT_EQ(if_block_end, ++if_block_it);
      }
    }

    // else if (os != "linux") { … }
    ASSERT_EQ(Node::CONDITION,
              (*block_it)->asCondition()->else_statement()->type());
    {
      auto* cond = (*block_it)->asCondition()->else_statement()->asCondition();

      // os != "linux"
      ASSERT_EQ(Node::BINARY_OP, cond->if_expression()->type());
      {
        auto* op = cond->if_expression()->asBinaryOp();

        // "!="
        ASSERT_EQ(Token::NOT_EQUAL, op->operation().type());

        // "os"
        CheckIdentifier(op->left_expression(), "os");

        // "linux"
        ASSERT_EQ(Node::LITERAL, op->right_expression()->type());
        {
          auto* right = op->right_expression()->asLiteral();

          ASSERT_EQ(Token::STRING, right->value().type());
          EXPECT_EQ("\"linux\"", right->value().value());
        }
      }

      // { … }
      ASSERT_EQ(Node::STATEMENT_LIST, cond->if_block()->type());
      {
        auto if_block_it = cond->if_block()->asStatementList()->begin(),
             if_block_end = cond->if_block()->asStatementList()->end();
        ASSERT_NE(if_block_end, if_block_it);

        // headers -= "test.h" + !public_headers
        ASSERT_EQ(Node::ASSIGNMENT, (*if_block_it)->type());
        {
          auto* assignment = (*if_block_it)->asAssignment();

          // "headers"
          CheckIdentifier(assignment->left_value(), "headers");

          // "-="
          EXPECT_EQ(Token::MINUS_EQUALS, assignment->operation().type());

          // "test.h" + !public_headers
          ASSERT_EQ(Node::BINARY_OP, assignment->right_value()->type());
          {
            auto* op = assignment->right_value()->asBinaryOp();

            // "+"
            ASSERT_EQ(Token::PLUS, op->operation().type());

            // "test.h"
            ASSERT_EQ(Node::LITERAL, op->left_expression()->type());
            {
              auto* literal = op->left_expression()->asLiteral();
              ASSERT_EQ(Token::STRING, literal->value().type());
              EXPECT_EQ("\"test.h\"", literal->value().value());
            }

            // "!public_headers"
            ASSERT_EQ(Node::NOT, op->right_expression()->type());
            CheckIdentifier(op->right_expression()->asNot()->expression(),
                            "public_headers");
          }
        }

        ASSERT_EQ(if_block_end, ++if_block_it);
      }
    }

    // else { … }
    ASSERT_EQ(Node::STATEMENT_LIST,
              (*block_it)
                  ->asCondition()
                  ->else_statement()
                  ->asCondition()
                  ->else_statement()
                  ->type());
    {
      auto if_block_it = (*block_it)
                             ->asCondition()
                             ->else_statement()
                             ->asCondition()
                             ->else_statement()
                             ->asStatementList()
                             ->begin(),
           if_block_end = (*block_it)
                              ->asCondition()
                              ->else_statement()
                              ->asCondition()
                              ->else_statement()
                              ->asStatementList()
                              ->end();
      ASSERT_NE(if_block_end, if_block_it);

      // testing = true && ( os == "win")
      ASSERT_EQ(Node::ASSIGNMENT, (*if_block_it)->type());
      {
        auto* assignment = (*if_block_it)->asAssignment();

        // "headers"
        CheckIdentifier(assignment->left_value(), "testing");

        // "="
        EXPECT_EQ(Token::EQUAL, assignment->operation().type());

        // true && ( os == "win")
        ASSERT_EQ(Node::BINARY_OP, assignment->right_value()->type());
        {
          auto* op = assignment->right_value()->asBinaryOp();

          // "&&"
          ASSERT_EQ(Token::BOOLEAN_AND, op->operation().type());

          // "true"
          ASSERT_EQ(Node::LITERAL, op->left_expression()->type());
          {
            auto* literal = op->left_expression()->asLiteral();

            ASSERT_EQ(Token::TRUE_TOKEN, literal->value().type());
            EXPECT_EQ("true", literal->value().value());
          }

          // os == "win"
          ASSERT_EQ(Node::BINARY_OP, op->right_expression()->type());
          {
            auto* comparison = op->right_expression()->asBinaryOp();

            // "=="
            ASSERT_EQ(Token::EQUAL_EQUAL, comparison->operation().type());

            // "os"
            CheckIdentifier(comparison->left_expression(), "os");

            // "win"
            ASSERT_EQ(Node::LITERAL, comparison->right_expression()->type());
            {
              auto* right = comparison->right_expression()->asLiteral();

              ASSERT_EQ(Token::STRING, right->value().type());
              EXPECT_EQ("\"win\"", right->value().value());
            }
          }
        }
      }

      ASSERT_EQ(if_block_end, ++if_block_it);
    }

    ASSERT_EQ(block_end, ++block_it);
  }

  ASSERT_EQ(stmt_end, ++stmt_it);
}

}  // namespace language::shi
}  // namespace shinobi
