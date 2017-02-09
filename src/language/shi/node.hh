#pragma once

#include <language/shi/token.hh>

namespace shinobi::language::shi {

class Node {};

using NodePtr = UniquePtr<Node>;

class ArrayAccessNode : public Node {
 public:
  ArrayAccessNode(const Token& id, NodePtr expr);
};

class AssignmentNode : public Node {
 public:
  AssignmentNode(const Token& op, NodePtr lvalue, NodePtr rvalue);
};

class BinaryOpNode : public Node {
 public:
  BinaryOpNode(const Token& op, NodePtr left_expr, NodePtr right_expr);
};

class CallNode : public Node {
 public:
  CallNode(const Token& id, NodePtr expr_list, NodePtr block);
};

class ConditionNode : public Node {
 public:
  ConditionNode(NodePtr if_expr, NodePtr if_block, NodePtr else_stmt);
};

class IdentifierNode : public Node {
 public:
  explicit IdentifierNode(const Token& id);
};

class NotNode : public Node {
 public:
  explicit NotNode(NodePtr expr);
};

class ScopeAccessNode : public Node {
 public:
  ScopeAccessNode(const Token& id, const Token& inner);
};

class StatementListNode : public Node {
 public:
  StatementListNode();

  void Append(NodePtr stmt);
};

}  // namespace shinobi::language::shi
