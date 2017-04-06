#pragma once

#include <base/location.hh>

namespace shinobi::language::shi {

class Token {
 public:
  enum Type {
    INVALID,

    // Literals
    INTEGER,      // 12345
    STRING,       // "blah"
    TRUE_TOKEN,   // true
    FALSE_TOKEN,  // false

    // Basic tokens and keywords
    IDENTIFIER,  // foo
    COMMA,       // ,
    IF_TOKEN,    // if
    ELSE_TOKEN,  // else

    // Unary operators
    BANG,  // !

    // Assignment operator
    EQUAL,  // =

    // Binary operators
    PLUS,              // +
    MINUS,             // -
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

    // Scope operator
    DOT,  // .

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

  using TypeList = List<Type>;

  Token() = default;
  Token(const Location& location, Type type, const String& value = String());
  Token(Token&&) = default;

  Token(const Token&) = delete;
  Token& operator=(const Token&) = delete;

  Type type() const { return type_; }
  const String& value() const { return value_; }
  const Location& location() const { return location_; }
  LocationRange range() const;
  ui8 precedence() const { return precedence_.at(type()); }

  static TypeList BinaryOps();
  static TypeList Literals();

  static String PrintType(Type type);

 private:
  const Location location_;
  const Type type_ = INVALID;
  const String value_;
  const static Map<Type, ui8> precedence_;
};

}  // namespace shinobi::language::shi
