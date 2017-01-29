#include <language/shi/parser.hh>

namespace shinobi::language::shi {

Parser::Parser(Iterator begin, Iterator end) : begin_(begin), end_(end) {}

bool Parser::Next(Token::Type type, ui32 advance) const {
  if (begin_ + advance == end_) {
    return false;
  }

  return (begin_ + advance)->type() == type;
}

Token Parser::Expect(Iterator it, List<Token::Type>&& expected_types) const {
  if (it == end_) {
    throw UnexpectedToken(expected_types);
  }

  bool good = false;

  for (const auto& type : expected_types) {
    if (it->type() == type) {
      good = true;
      break;
    }
  }

  if (!good) {
    throw UnexpectedToken(expected_types);
  }

  return *it;
}

Token Parser::Consume(List<Token::Type>&& expected_types) {
  auto token = Expect(begin_, std::move(expected_types));
  ++begin_;
  return token;
}

/*
 * Parsing methods in alphabetical order
 */

Parser::NodePtr Parser::ParseAssignment() {
  auto lvalue = ParseLValue();
  auto op = Consume({Token::EQUAL, Token::PLUS_EQUALS, Token::MINUS_EQUALS});
  auto rvalue = ParseExpression();

  return std::make_unique<AssignmentNode>(op.type(), std::move(lvalue),
                                          std::move(rvalue));
}

Parser::NodePtr Parser::ParseBlock() {
  Consume({Token::LEFT_BRACE});

  auto list = ParseStatementList();

  Consume({Token::RIGHT_BRACE});

  return list;
}

Parser::NodePtr Parser::ParseCall() {
  auto id = Consume({Token::IDENTIFIER});

  Consume({Token::LEFT_PAREN});

  NodePtr expr_list;
  if (!Next(Token::RIGHT_PAREN)) {
    expr_list = ParseExpressionList();
  }

  Consume({Token::RIGHT_PAREN});

  NodePtr block;
  if (Next(Token::LEFT_BRACE)) {
    block = ParseBlock();
  }

  return std::make_unique<CallNode>(id.value(), std::move(expr_list),
                                    std::move(block));
}

Parser::NodePtr Parser::ParseCondition() {
  CHECK(begin_->type() == Token::IF_TOKEN);

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

Parser::NodePtr Parser::ParseLValue() {
  auto id = Consume({Token::IDENTIFIER});

  if (Next(Token::LEFT_BRACKET)) {
    Consume({Token::LEFT_BRACKET});
    auto expr = ParseExpression();
    Consume({Token::RIGHT_BRACKET});

    return std::make_unique<ArrayAccessNode>(id.value(), std::move(expr));
  }

  if (Next(Token::DOT)) {
    return std::make_unique<ScopeAccessNode>(
        id.value(), Consume({Token::IDENTIFIER}).value());
  }

  return std::make_unique<IdentifierNode>(id.value());
}

Parser::NodePtr Parser::ParseStatement() {
  Expect(begin_, {Token::IF_TOKEN, Token::IDENTIFIER});

  if (Next(Token::IF_TOKEN)) {
    return ParseCondition();
  }

  if (Next(Token::IDENTIFIER)) {
    Expect(begin_ + 1, {Token::EQUAL, Token::LEFT_PAREN});

    if (Next(Token::EQUAL, 1)) {
      return ParseAssignment();
    }

    if (Next(Token::LEFT_PAREN, 1)) {
      return ParseCall();
    }
  }

  NOTREACHED();
}

Parser::NodePtr Parser::ParseStatementList() {
  auto list = std::make_unique<StatementListNode>();

  while (begin_ != end_) {
    list.Append(ParseStatement());
  }

  return list;
}

}  // namespace shinobi::language::shi
