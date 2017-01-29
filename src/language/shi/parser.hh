#pragma once

#include <language/shi/token.hh>

/* Language Shi Grammar.

      The input tokens form a syntax tree following a context-free grammar:

          File = StatementList .

          Statement     = Assignment | Call | Condition .
          LValue        = identifier | ArrayAccess | ScopeAccess .
          Assignment    = LValue AssignOp Expr .
          Call          = identifier "(" [ ExprList ] ")" [ Block ] .
          Condition     = "if" "(" Expr ")" Block
                          [ "else" ( Condition | Block ) ] .
          Block         = "{" StatementList "}" .
          StatementList = { Statement } .

          ArrayAccess = identifier "[" Expr "]" .
          ScopeAccess = identifier "." identifier .
          Expr        = UnaryExpr | Expr BinaryOp Expr .
          UnaryExpr   = PrimaryExpr | UnaryOp UnaryExpr .
          PrimaryExpr = identifier | integer | string | Call
                      | ArrayAccess | ScopeAccess | Block
                      | "(" Expr ")"
                      | "[" [ ExprList [ "," ] ] "]" .
          ExprList    = Expr { "," Expr } .

          AssignOp = "=" | "+=" | "-=" .
          UnaryOp  = "!" .
          BinaryOp = "+" | "-"                  // highest priority
                   | "<" | "<=" | ">" | ">="
                   | "==" | "!="
                   | "&&"
                   | "||" .                     // lowest priority

      All binary operators are left-associative.
 */

namespace shinobi::language::shi {

class Parser {
 public:
  using Iterator = Vector<Token>::const_iterator;
  using NodePtr = UniquePtr<Node>;

  Parser(Iterator begin, Iterator end);

  inline NodePtr Parse() { return ParseStatementList(); }

 private:
  bool Next(Token::Type type, ui32 advance = 0u) const;
  Token Expect(Iterator it, List<Token::Type>&& expected_types) const;
  Token Consume(List<Token::Type>&& expected_types);

  NodePtr ParseAssignment();
  NodePtr ParseBlock();
  NodePtr ParseCall();
  NodePtr ParseCondition();
  NodePtr ParseLValue();
  NodePtr ParseStatement();
  NodePtr ParseStatementList();

  Iterator begin_;
  const Iterator end_;
};

}  // namespace shinobi::language::shi
