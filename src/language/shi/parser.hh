#pragma once

#include <language/shi/node.hh>
#include <language/shi/token.hh>

/* Language Shi Grammar.

      The input tokens form a syntax tree following a context-free grammar:

          File = StatementList .

          StatementList = { Statement } .
          Statement     = Assignment | Call | Condition .
          Assignment    = LValue AssignOp Expr .
          Call          = identifier "(" [ ExprList ] ")" [ Block ] .
          Condition     = "if" "(" Expr ")" Block
                          [ "else" ( Condition | Block ) ] .
          Block         = "{" StatementList "}" .

          LValue      = identifier | ArrayAccess | ScopeAccess .
          ArrayAccess = identifier "[" Expr "]" .
          ScopeAccess = identifier "." identifier .
          ExprList    = Expr { "," Expr } .
          Expr        = UnaryExpr | Expr BinaryOp Expr .
          UnaryExpr   = PrimaryExpr | UnaryOp UnaryExpr .
          PrimaryExpr = LValue | integer | string | Call | "(" Expr ")"
                      | "[" [ ExprList [ "," ] ] "]" .
                      # Call without Block only.

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

  Parser(Iterator begin, Iterator end);

  inline NodePtr Parse() { return ParseStatementList(); }

 private:
  bool Next(Token::Type type, ui32 advance = 0u) const;
  bool Next(const Token::TypeList& types) const;
  const Token& Expect(const Token::TypeList& expected_types,
                      ui32 advance = 0u) const;
  const Token& Consume(const Token::TypeList& expected_types);

  NodePtr ParseAssignment();
  NodePtr ParseBlock();
  NodePtr ParseCall(bool expect_block);
  NodePtr ParseCondition();
  NodePtr ParseExpression(ui8 precedence = 0u);
  NodePtr ParseExpressionList();
  NodePtr ParseLiteral();
  NodePtr ParseLValue();
  NodePtr ParseStatement();
  NodePtr ParseStatementList();

  Iterator begin_;
  const Iterator end_;
};

}  // namespace shinobi::language::shi
