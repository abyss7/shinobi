#include <language/shi/token.hh>

#include <base/assert.hh>

namespace shinobi::language::shi {

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
