#pragma once

#include <language/shi/token.hh>

namespace shinobi::language::shi {

class ArrayAccessNode;
class AssignmentNode;
class BinaryOpNode;
class CallNode;
class ConditionNode;
class ExpressionListNode;
class IdentifierNode;
class LiteralNode;
class NotNode;
class ScopeAccessNode;
class StatementListNode;

class Node {
 public:
  // Need explicit node types for |Writer|.
  enum Type {
    ARRAY_ACCESS,
    ASSIGNMENT,
    BINARY_OP,
    CALL,
    CONDITION,
    EXPRESSION_LIST,
    IDENTIFIER,
    LITERAL,
    NOT,
    SCOPE_ACCESS,
    STATEMENT_LIST,
  };

  explicit Node(Type type) : type_(type) {}
  virtual ~Node() {}

  inline Type type() const { return type_; }

  const ArrayAccessNode* asArrayAccess() const;
  const AssignmentNode* asAssignment() const;
  const BinaryOpNode* asBinaryOp() const;
  const CallNode* asCall() const;
  const ConditionNode* asCondition() const;
  const ExpressionListNode* asExpressionList() const;
  const IdentifierNode* asIdentifier() const;
  const LiteralNode* asLiteral() const;
  const NotNode* asNot() const;
  const ScopeAccessNode* asScopeAccess() const;
  const StatementListNode* asStatementList() const;

 private:
  const Type type_;
};

using NodePtr = UniquePtr<Node>;

class ArrayAccessNode : public Node {
 public:
  ArrayAccessNode(const Token& id, NodePtr expr);

  inline const Token& identifier() const { return id_; }
  inline const Node* expression() const { return expr_.get(); }

 private:
  const Token& id_;
  NodePtr expr_;
};

class AssignmentNode : public Node {
 public:
  AssignmentNode(const Token& op, NodePtr lvalue, NodePtr rvalue);

  inline const Token& operation() const { return op_; }
  inline const Node* left_value() const { return lvalue_.get(); }
  inline const Node* right_value() const { return rvalue_.get(); }

 private:
  const Token& op_;
  NodePtr lvalue_, rvalue_;
};

class BinaryOpNode : public Node {
 public:
  BinaryOpNode(const Token& op, NodePtr lexpr, NodePtr rexpr);

  const Token& operation() const { return op_; }
  inline const Node* left_expression() const { return lexpr_.get(); }
  inline const Node* right_expression() const { return rexpr_.get(); }

 private:
  const Token& op_;
  NodePtr lexpr_, rexpr_;
};

class CallNode : public Node {
 public:
  CallNode(const Token& id, NodePtr expr_list, NodePtr block);

  const Token& identifier() const { return id_; }
  inline const Node* expression_list() const { return expr_list_.get(); }
  inline const Node* block() const { return block_.get(); }

 private:
  const Token& id_;
  NodePtr expr_list_, block_;
};

class ConditionNode : public Node {
 public:
  ConditionNode(NodePtr if_expr, NodePtr if_block, NodePtr else_stmt);

  inline const Node* if_expression() const { return if_expr_.get(); }
  inline const Node* if_block() const { return if_block_.get(); }
  inline const Node* else_statement() const { return else_stmt_.get(); }

 private:
  NodePtr if_expr_, if_block_, else_stmt_;
};

class ExpressionListNode : public Node {
 public:
  ExpressionListNode() : Node(EXPRESSION_LIST) {}

  void Append(NodePtr expr);

  inline List<NodePtr>::const_iterator begin() const { return exprs_.begin(); }
  inline List<NodePtr>::const_iterator end() const { return exprs_.end(); }

 private:
  List<NodePtr> exprs_;
};

class IdentifierNode : public Node {
 public:
  explicit IdentifierNode(const Token& id);

  inline const Token& identifier() const { return id_; }

 private:
  const Token& id_;
};

class LiteralNode : public Node {
 public:
  explicit LiteralNode(const Token& value);

  inline const Token& value() const { return value_; }

 private:
  const Token& value_;
};

class NotNode : public Node {
 public:
  explicit NotNode(NodePtr expr);

  inline const Node* expression() const { return expr_.get(); }

 private:
  NodePtr expr_;
};

class ScopeAccessNode : public Node {
 public:
  ScopeAccessNode(const Token& id, const Token& inner);

  inline const Token& identifier() const { return id_; }
  inline const Token& inner() const { return inner_; }

 private:
  const Token &id_, &inner_;
};

class StatementListNode : public Node {
 public:
  StatementListNode() : Node(STATEMENT_LIST) {}

  void Append(NodePtr stmt);

  inline List<NodePtr>::const_iterator begin() const { return stmts_.begin(); }
  inline List<NodePtr>::const_iterator end() const { return stmts_.end(); }

 private:
  List<NodePtr> stmts_;
};

}  // namespace shinobi::language::shi
