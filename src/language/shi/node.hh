#pragma once

#include <language/shi/token.hh>

namespace shinobi::language::shi {

class Node {};

using NodePtr = UniquePtr<Node>;

class ArrayAccessNode : public Node {
 public:
  ArrayAccessNode(const Token& id, NodePtr expr);

 private:
  const Token& id_;
  NodePtr expr_;
};

class AssignmentNode : public Node {
 public:
  AssignmentNode(const Token& op, NodePtr lvalue, NodePtr rvalue);

 private:
  const Token& op_;
  NodePtr lvalue_, rvalue_;
};

class BinaryOpNode : public Node {
 public:
  BinaryOpNode(const Token& op, NodePtr lexpr, NodePtr rexpr);

 private:
  const Token& op_;
  NodePtr lexpr_, rexpr_;
};

class CallNode : public Node {
 public:
  CallNode(const Token& id, NodePtr expr_list, NodePtr block);

 private:
  const Token& id_;
  NodePtr expr_list_, block_;
};

class ConditionNode : public Node {
 public:
  ConditionNode(NodePtr if_expr, NodePtr if_block, NodePtr else_stmt);

 private:
  NodePtr if_expr_, if_block_, else_stmt_;
};

class IdentifierNode : public Node {
 public:
  explicit IdentifierNode(const Token& id);

 private:
  const Token& id_;
};

class LiteralNode : public Node {
 public:
  explicit LiteralNode(const Token& id);

 private:
  const Token& id_;
};

class NotNode : public Node {
 public:
  explicit NotNode(NodePtr expr);

 private:
  NodePtr expr_;
};

class ScopeAccessNode : public Node {
 public:
  ScopeAccessNode(const Token& id, const Token& inner);

 private:
  const Token &id_, &inner_;
};

class StatementListNode : public Node {
 public:
  void Append(NodePtr stmt);

 private:
  List<NodePtr> stmts_;
};

}  // namespace shinobi::language::shi
