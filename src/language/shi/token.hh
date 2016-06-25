#pragma once

#include <base/location.hh>

namespace shinobi::language::shi {

class Token {
 public:
  enum Type {
    INVALID,

    // Basic tokens and keywords
    INTEGER,      // 12345
    STRING,       // "blah"
    IDENTIFIER,   // foo
    TRUE_TOKEN,   // true
    FALSE_TOKEN,  // false
    COMMA,        // ,
    IF_TOKEN,     // if
    ELSE_TOKEN,   // else

    // Unary operators
    PLUS,   // +
    MINUS,  // -
    BANG,   // !

    // Binary operators
    EQUAL,             // =
    PLUS_EQUALS,       // +=
    MINUS_EQUALS,      // -=
    EQUAL_EQUAL,       // ==
    NOT_EQUAL,         // !=
    LESS_EQUAL,        // <=
    GREATER_EQUAL,     // >=
    STRICTLY_LESS,     // <
    STRICTLY_GREATER,  // >
    BOOLEAN_AND,       // &&
    BOOLEAN_OR,        // ||
    DOT,               // .

    // Brackets
    LEFT_PAREN,     // (
    RIGHT_PAREN,    // )
    LEFT_BRACKET,   // [
    RIGHT_BRACKET,  // ]
    LEFT_BRACE,     // {
    RIGHT_BRACE,    // }

    // Comments
    COMMENT,  // #...\n

    TYPE_SIZE
  };

  Token() = default;
  Token(const Location& location, Type type, const String& value = String());
  Token(Token&&) = default;

  Token(const Token&) = delete;
  Token& operator=(const Token&) = delete;

  Type type() const { return type_; }
  const String& value() const {
    // TODO: for known token types value should be auto-generated.
    return value_;
  }
  const Location& location() const { return location_; }

  LocationRange range() const;

 private:
  const Location location_;
  const Type type_ = INVALID;
  const String value_;
};

}  // namespace shinobi::language::shi
