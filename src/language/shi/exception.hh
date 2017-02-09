#pragma once

#include <language/shi/token.hh>

namespace shinobi::language::shi {

class UnexpectedToken : public std::exception {
 public:
  explicit UnexpectedToken(const Token::TypeList& expected_types);
};

}  // namespace shinobi::language::shi
