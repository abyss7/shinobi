#include <language/shi/parser.hh>

#include <base/assert.hh>
#include <language/shi/exception.hh>

namespace shinobi::language::shi {

Parser::Parser(Iterator begin, Iterator end) : begin_(begin), end_(end) {}

bool Parser::Next(Token::Type type, ui32 advance) const {
  // Expect that we don't jump out of range.
  if (begin_ + advance == end_) {
    return false;
  }

  return (begin_ + advance)->type() == type;
}

bool Parser::Next(const Token::TypeList& types) const {
  for (const auto& type : types) {
    if (Next(type)) {
      return true;
    }
  }

  return false;
}

const Token& Parser::Expect(const Token::TypeList& expected_types,
                            ui32 advance) const {
  auto it = begin_ + advance;

  if (it == end_) {
    // TODO: pass location of the end of tokens.
    throw UnexpectedEndOfTokens(expected_types, Location());
  }

  bool good = false;

  for (const auto& type : expected_types) {
    if (it->type() == type) {
      good = true;
      break;
    }
  }

  if (!good) {
    throw UnexpectedToken(*it, expected_types);
  }

  return *it;
}

const Token& Parser::Consume(const Token::TypeList& expected_types) {
  const auto& token = Expect(expected_types);
  ++begin_;
  return token;
}

/*
 * Parsing methods in alphabetical order
 */

NodePtr Parser::ParseAssignment() {
  auto lvalue = ParseLValue();
  const auto& op =
      Consume({Token::EQUAL, Token::PLUS_EQUALS, Token::MINUS_EQUALS});
  auto rvalue = ParseExpression();

  return std::make_unique<AssignmentNode>(op, std::move(lvalue),
                                          std::move(rvalue));
}

NodePtr Parser::ParseBlock() {
  Consume({Token::LEFT_BRACE});
  auto list = ParseStatementList();
  Consume({Token::RIGHT_BRACE});

  return list;
}

NodePtr Parser::ParseCall(bool expect_block) {
  const auto& id = Consume({Token::IDENTIFIER});

  Consume({Token::LEFT_PAREN});

  NodePtr expr_list;
  if (!Next(Token::RIGHT_PAREN)) {
    expr_list = ParseExpressionList();
  }

  Consume({Token::RIGHT_PAREN});

  NodePtr block;
  if (expect_block && Next(Token::LEFT_BRACE)) {
    block = ParseBlock();
  }

  return std::make_unique<CallNode>(id, std::move(expr_list), std::move(block));
}

NodePtr Parser::ParseCondition() {
  Consume({Token::IF_TOKEN});
  Consume({Token::LEFT_PAREN});

  auto if_expr = ParseExpression();

  Consume({Token::RIGHT_PAREN});

  auto if_block = ParseBlock();

  NodePtr else_stmt;
  if (Next(Token::ELSE_TOKEN)) {
    Consume({Token::ELSE_TOKEN});

    if (Next(Token::IF_TOKEN)) {
      else_stmt = ParseCondition();
    } else {
      else_stmt = ParseBlock();
    }
  }

  return std::make_unique<ConditionNode>(
      std::move(if_expr), std::move(if_block), std::move(else_stmt));
}

NodePtr Parser::ParseExpression(ui8 precedence) {
  NodePtr left;

  if (Next(Token::IDENTIFIER)) {
    if (Next(Token::LEFT_PAREN, 1)) {
      left = ParseCall(false);
    } else {
      left = ParseLValue();
    }
  } else if (Next(Token::LEFT_PAREN)) {
    Consume({Token::LEFT_PAREN});
    left = ParseExpression();
    Consume({Token::RIGHT_PAREN});
  } else if (Next(Token::LEFT_BRACKET)) {
    Consume({Token::LEFT_BRACKET});
    left = ParseExpressionList();
    Consume({Token::RIGHT_BRACKET});
  } else if (Next(Token::BANG)) {
    const auto& not_token = Consume({Token::BANG});
    const auto& not_precedence = not_token.precedence();
    DCHECK(precedence <= not_precedence);
    return std::make_unique<NotNode>(ParseExpression(not_precedence));
  } else if (Next(Token::Literals())) {
    left = ParseLiteral();
  } else {
    return left;
  }

  while (Next(Token::BinaryOps())) {
    if (Expect(Token::BinaryOps()).precedence() <= precedence) {
      return left;
    }

    const auto& op = Consume(Token::BinaryOps());
    auto right = ParseExpression(op.precedence());
    left =
        std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right));
  }

  return left;
}

NodePtr Parser::ParseExpressionList() {
  auto list = std::make_unique<StatementListNode>();

  auto expr = ParseExpression();
  while (Next(Token::COMMA)) {
    list->Append(std::move(expr));
    Consume({Token::COMMA});
    expr = ParseExpression();
  }

  list->Append(std::move(expr));

  return list;
}

NodePtr Parser::ParseLiteral() {
  const auto& value = Consume(Token::Literals());
  return std::make_unique<LiteralNode>(value);
}

NodePtr Parser::ParseLValue() {
  const auto& id = Consume({Token::IDENTIFIER});

  if (Next(Token::LEFT_BRACKET)) {
    Consume({Token::LEFT_BRACKET});
    auto expr = ParseExpression();
    Consume({Token::RIGHT_BRACKET});

    return std::make_unique<ArrayAccessNode>(id, std::move(expr));
  }

  if (Next(Token::DOT)) {
    return std::make_unique<ScopeAccessNode>(id, Consume({Token::IDENTIFIER}));
  }

  return std::make_unique<IdentifierNode>(id);
}

NodePtr Parser::ParseStatement() {
  if (Next(Token::IF_TOKEN)) {
    return ParseCondition();
  }

  if (Next(Token::IDENTIFIER)) {
    Expect({Token::EQUAL, Token::PLUS_EQUALS, Token::MINUS_EQUALS,
            Token::LEFT_PAREN},
           1);

    if (Next(Token::LEFT_PAREN, 1)) {
      return ParseCall(true);
    } else {
      return ParseAssignment();
    }
  }

  return NodePtr();
}

NodePtr Parser::ParseStatementList() {
  auto list = std::make_unique<StatementListNode>();

  while (begin_ != end_) {
    NodePtr stmt = ParseStatement();
    if (!stmt) {
      // Statement list is over.
      break;
    }
    list->Append(std::move(stmt));
  }

  return list;
}

}  // namespace shinobi::language::shi
