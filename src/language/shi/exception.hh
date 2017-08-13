#pragma once

#include <language/shi/token.hh>

namespace shinobi::language::shi {

/*
 * Possible syntactic errors are:
 *  - unexpected symbol: the only case when we know expected symbol is a '"' at
 *      end of the string.
 *  - unexpected end of stream: only possible with a string.
 *  - unexpected token: in many situations we know what we expect.
 *  - unexpected end of tokens: may happen in a lot of situations too.
 */

class SyntaxError : public std::exception {
 public:
  explicit SyntaxError(const Location& location) : location_(location) {}
  const char* what() const noexcept override;

 protected:
  inline void SetUnexpected(const String& unexpected) {
    unexpected_ = unexpected;
  }
  inline void SetExpected(const String& expected) { expected_ = expected; }

 private:
  const Location location_;
  mutable String message_;
  String unexpected_, expected_;
};

class UnexpectedSymbol : public SyntaxError {
 public:
  UnexpectedSymbol(const char symbol, const Location& location);
  UnexpectedSymbol(const char symbol, const Location& location,
                   List<char> expected);
};

class UnexpectedToken : public SyntaxError {
 public:
  explicit UnexpectedToken(const Token& token);
  UnexpectedToken(const Token& token, const Token::TypeList& expected);
};

class UnexpectedEndOfTokens : public SyntaxError {
 public:
  UnexpectedEndOfTokens(const Token::TypeList& expected_types,
                        const Location& location);
};

class SemanticError : public std::exception {
 public:
  SemanticError(const Location& location, const String& error_message);
  const char* what() const noexcept override;

 private:
  const Location location_;
  String message_;
};

}  // namespace shinobi::language::shi
