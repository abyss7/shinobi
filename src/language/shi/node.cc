#include <language/shi/node.hh>

namespace shinobi::language::shi {

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

LiteralNode::LiteralNode(const Token& id) : id_(id) {}

NotNode::NotNode(NodePtr expr) : expr_(std::move(expr)) {}

ScopeAccessNode::ScopeAccessNode(const Token& id, const Token& inner)
    : id_(id), inner_(inner) {}

void StatementListNode::Append(NodePtr stmt) {
  stmts_.emplace_back(std::move(stmt));
}

}  // namespace shinobi::language::shi
