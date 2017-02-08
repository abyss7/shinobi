#include <language/shi/token.hh>

#include <base/assert.hh>

namespace shinobi::language::shi {

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
    : location_(location), type_(type), value_(value) {
  DCHECK(location_);
  DCHECK(type_ != INVALID);
}

LocationRange Token::range() const {
  return LocationRange(location_,
                       Location(location_.file_path(), location_.line(),
                                location_.column() + value().size()));
}

}  // namespace shinobi::language::shi
