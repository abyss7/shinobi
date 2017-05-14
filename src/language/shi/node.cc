#include <language/shi/node.hh>

#include <base/assert.hh>

namespace shinobi::language::shi {

const ArrayAccessNode* Node::asArrayAccess() const {
  DCHECK(dynamic_cast<const ArrayAccessNode*>(this));
  CHECK(type() == ARRAY_ACCESS);
  return static_cast<const ArrayAccessNode*>(this);
}

const AssignmentNode* Node::asAssignment() const {
  DCHECK(dynamic_cast<const AssignmentNode*>(this));
  CHECK(type() == ASSIGNMENT);
  return static_cast<const AssignmentNode*>(this);
}

const BinaryOpNode* Node::asBinaryOp() const {
  DCHECK(dynamic_cast<const BinaryOpNode*>(this));
  CHECK(type() == BINARY_OP);
  return static_cast<const BinaryOpNode*>(this);
}

const CallNode* Node::asCall() const {
  DCHECK(dynamic_cast<const CallNode*>(this));
  CHECK(type() == CALL);
  return static_cast<const CallNode*>(this);
}

const ConditionNode* Node::asCondition() const {
  DCHECK(dynamic_cast<const ConditionNode*>(this));
  CHECK(type() == CONDITION);
  return static_cast<const ConditionNode*>(this);
}

const ExpressionListNode* Node::asExpressionList() const {
  DCHECK(dynamic_cast<const ExpressionListNode*>(this));
  CHECK(type() == EXPRESSION_LIST);
  return static_cast<const ExpressionListNode*>(this);
}

const IdentifierNode* Node::asIdentifier() const {
  DCHECK(dynamic_cast<const IdentifierNode*>(this));
  CHECK(type() == IDENTIFIER);
  return static_cast<const IdentifierNode*>(this);
}

const LiteralNode* Node::asLiteral() const {
  DCHECK(dynamic_cast<const LiteralNode*>(this));
  CHECK(type() == LITERAL);
  return static_cast<const LiteralNode*>(this);
}

const NotNode* Node::asNot() const {
  DCHECK(dynamic_cast<const NotNode*>(this));
  CHECK(type() == NOT);
  return static_cast<const NotNode*>(this);
}

const ScopeAccessNode* Node::asScopeAccess() const {
  DCHECK(dynamic_cast<const ScopeAccessNode*>(this));
  CHECK(type() == SCOPE_ACCESS);
  return static_cast<const ScopeAccessNode*>(this);
}

const StatementListNode* Node::asStatementList() const {
  DCHECK(dynamic_cast<const StatementListNode*>(this));
  CHECK(type() == STATEMENT_LIST);
  return static_cast<const StatementListNode*>(this);
}

ArrayAccessNode::ArrayAccessNode(const Token& id, NodePtr expr)
    : Node(ARRAY_ACCESS), id_(id), expr_(std::move(expr)) {}

AssignmentNode::AssignmentNode(const Token& op, NodePtr lvalue, NodePtr rvalue)
    : Node(ASSIGNMENT),
      op_(op),
      lvalue_(std::move(lvalue)),
      rvalue_(std::move(rvalue)) {}

BinaryOpNode::BinaryOpNode(const Token& op, NodePtr lexpr, NodePtr rexpr)
    : Node(BINARY_OP),
      op_(op),
      lexpr_(std::move(lexpr)),
      rexpr_(std::move(rexpr)) {}

CallNode::CallNode(const Token& id, NodePtr expr_list, NodePtr block)
    : Node(CALL),
      id_(id),
      expr_list_(std::move(expr_list)),
      block_(std::move(block)) {}

ConditionNode::ConditionNode(NodePtr if_expr, NodePtr if_block,
                             NodePtr else_stmt)
    : Node(CONDITION),
      if_expr_(std::move(if_expr)),
      if_block_(std::move(if_block)),
      else_stmt_(std::move(else_stmt)) {}

void ExpressionListNode::Append(NodePtr expr) {
  exprs_.emplace_back(std::move(expr));
}

IdentifierNode::IdentifierNode(const Token& id) : Node(IDENTIFIER), id_(id) {}

LiteralNode::LiteralNode(const Token& value) : Node(LITERAL), value_(value) {}

NotNode::NotNode(NodePtr expr) : Node(NOT), expr_(std::move(expr)) {}

ScopeAccessNode::ScopeAccessNode(const Token& id, const Token& inner)
    : Node(SCOPE_ACCESS), id_(id), inner_(inner) {}

void StatementListNode::Append(NodePtr stmt) {
  stmts_.emplace_back(std::move(stmt));
}

}  // namespace shinobi::language::shi
