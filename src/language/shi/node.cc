#include <language/shi/node.hh>

#include <base/assert.hh>

namespace shinobi::language::shi {

const ArrayAccessNode* Node::asArrayAccess() const {
  DCHECK(dynamic_cast<const ArrayAccessNode*>(this));
  return static_cast<const ArrayAccessNode*>(this);
}

const AssignmentNode* Node::asAssignment() const {
  DCHECK(dynamic_cast<const AssignmentNode*>(this));
  return static_cast<const AssignmentNode*>(this);
}

const BinaryOpNode* Node::asBinaryOp() const {
  DCHECK(dynamic_cast<const BinaryOpNode*>(this));
  return static_cast<const BinaryOpNode*>(this);
}

const CallNode* Node::asCall() const {
  DCHECK(dynamic_cast<const CallNode*>(this));
  return static_cast<const CallNode*>(this);
}

const ConditionNode* Node::asCondition() const {
  DCHECK(dynamic_cast<const ConditionNode*>(this));
  return static_cast<const ConditionNode*>(this);
}

const IdentifierNode* Node::asIdentifier() const {
  DCHECK(dynamic_cast<const IdentifierNode*>(this));
  return static_cast<const IdentifierNode*>(this);
}

const LiteralNode* Node::asLiteral() const {
  DCHECK(dynamic_cast<const LiteralNode*>(this));
  return static_cast<const LiteralNode*>(this);
}

const NotNode* Node::asNot() const {
  DCHECK(dynamic_cast<const NotNode*>(this));
  return static_cast<const NotNode*>(this);
}

const ScopeAccessNode* Node::asScopeAccess() const {
  DCHECK(dynamic_cast<const ScopeAccessNode*>(this));
  return static_cast<const ScopeAccessNode*>(this);
}

const StatementListNode* Node::asStatementList() const {
  DCHECK(dynamic_cast<const StatementListNode*>(this));
  return static_cast<const StatementListNode*>(this);
}

ArrayAccessNode::ArrayAccessNode(const Token& id, NodePtr expr)
    : id_(id), expr_(std::move(expr)) {}

AssignmentNode::AssignmentNode(const Token& op, NodePtr lvalue, NodePtr rvalue)
    : op_(op), lvalue_(std::move(lvalue)), rvalue_(std::move(rvalue)) {}

BinaryOpNode::BinaryOpNode(const Token& op, NodePtr lexpr, NodePtr rexpr)
    : op_(op), lexpr_(std::move(lexpr)), rexpr_(std::move(rexpr)) {}

CallNode::CallNode(const Token& id, NodePtr expr_list, NodePtr block)
    : id_(id), expr_list_(std::move(expr_list)), block_(std::move(block)) {}

ConditionNode::ConditionNode(NodePtr if_expr, NodePtr if_block,
                             NodePtr else_stmt)
    : if_expr_(std::move(if_expr)),
      if_block_(std::move(if_block)),
      else_stmt_(std::move(else_stmt)) {}

IdentifierNode::IdentifierNode(const Token& id) : id_(id) {}

LiteralNode::LiteralNode(const Token& value) : value_(value) {}

NotNode::NotNode(NodePtr expr) : expr_(std::move(expr)) {}

ScopeAccessNode::ScopeAccessNode(const Token& id, const Token& inner)
    : id_(id), inner_(inner) {}

void StatementListNode::Append(NodePtr stmt) {
  stmts_.emplace_back(std::move(stmt));
}

}  // namespace shinobi::language::shi
