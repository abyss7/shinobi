#include <language/shi/token.hh>

#include <base/assert.hh>

namespace shinobi::language::shi {

// static
Token::TypeList Token::BinaryOps() {
  return {
      Token::PLUS,          Token::MINUS,
      Token::EQUAL_EQUAL,   Token::NOT_EQUAL,
      Token::LESS_EQUAL,    Token::GREATER_EQUAL,
      Token::STRICTLY_LESS, Token::STRICTLY_GREATER,
      Token::BOOLEAN_AND,   Token::BOOLEAN_OR,
  };
}

// static
Token::TypeList Token::Literals() {
  return {
      Token::INTEGER, Token::STRING, Token::TRUE_TOKEN, Token::FALSE_TOKEN,
  };
}

// static
String Token::PrintType(Token::Type type) {
  switch (type) {
    case INTEGER:
      return "integer literal";
    case STRING:
      return "string literal";
    case TRUE_TOKEN:
      return "\"true\" keyword";
    case FALSE_TOKEN:
      return "\"false\" keyword";
    case IDENTIFIER:
      return "identifier";
    case COMMA:
      return "comma";
    case IF_TOKEN:
      return "\"if\" keyword";
    case ELSE_TOKEN:
      return "\"else\" keyword";
    case BANG:
      return "logical \"not\"";
    case EQUAL:
      return "assignment";
    case PLUS:
      return "addition";
    case MINUS:
      return "substraction";
    default:
      // TODO: add strings for all types.
      return "TODO:UNKNOWN";
  }
}

// static
const Map<Token::Type, ui8> Token::precedence_ = {
    {Token::EQUAL, 1},
    {Token::PLUS_EQUALS, 1},
    {Token::MINUS_EQUALS, 1},
    {Token::BOOLEAN_OR, 2},
    {Token::BOOLEAN_AND, 3},
    {Token::EQUAL_EQUAL, 4},
    {Token::NOT_EQUAL, 4},
    {Token::LESS_EQUAL, 5},
    {Token::GREATER_EQUAL, 5},
    {Token::STRICTLY_LESS, 5},
    {Token::STRICTLY_GREATER, 5},
    {Token::PLUS, 6},
    {Token::MINUS, 6},
    {Token::BANG, 7},
    {Token::DOT, 8},  // not used
};

Token::Token(const Location& location, Type type, const String& value)
    : location_(location), type_(type), value_([&value, &type]() -> String {
        switch (type) {
          case TRUE_TOKEN:
            return "true";
          case FALSE_TOKEN:
            return "false";
          case COMMA:
            return ",";
          case IF_TOKEN:
            return "if";
          case ELSE_TOKEN:
            return "else";
          case BANG:
            return "!";
          case EQUAL:
            return "=";
          case PLUS:
            return "+";
          case MINUS:
            return "-";
          case PLUS_EQUALS:
            return "+=";
          case MINUS_EQUALS:
            return "-=";
          case EQUAL_EQUAL:
            return "==";
          case NOT_EQUAL:
            return "!=";
          case LESS_EQUAL:
            return "<=";
          case GREATER_EQUAL:
            return ">=";
          case STRICTLY_LESS:
            return "<";
          case STRICTLY_GREATER:
            return ">";
          case BOOLEAN_AND:
            return "&&";
          case BOOLEAN_OR:
            return "||";
          case DOT:
            return ".";
          case LEFT_PAREN:
            return "(";
          case RIGHT_PAREN:
            return ")";
          case LEFT_BRACKET:
            return "[";
          case RIGHT_BRACKET:
            return "]";
          case LEFT_BRACE:
            return "{";
          case RIGHT_BRACE:
            return "}";
          default:
            return value;
        }
      }()) {
  DCHECK(location_);
  DCHECK(type_ != INVALID);
}

LocationRange Token::range() const {
  return LocationRange(location_,
                       Location(location_.file_path(), location_.line(),
                                location_.column() + value().size(),
                                location_.byte() + value().size()));
}

}  // namespace shinobi::language::shi
