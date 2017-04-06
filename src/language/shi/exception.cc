#include <language/shi/exception.hh>

#include STL(cctype)
#include STL(iomanip)
#include STL(sstream)

namespace shinobi::language::shi {

const char* SyntaxError::what() const noexcept {
  message_ = "Syntax error: unexpected " + unexpected_ + " at " +
             location_.file_path() + ":" + std::to_string(location_.line()) +
             ":" + std::to_string(location_.column());
  if (!expected_.empty()) {
    message_ += ", expected " + expected_;
  }
  return message_.c_str();
}

UnexpectedSymbol::UnexpectedSymbol(const char symbol, const Location& location)
    : SyntaxError(location) {
  if (std::isprint(symbol)) {
    SetUnexpected("symbol " + String(1, symbol));
  } else {
    std::stringstream ss;
    ss << "\\x" << std::hex << std::setfill('0') << std::setw(2) << symbol;
    SetUnexpected("symbol " + ss.str());
  }
}

UnexpectedSymbol::UnexpectedSymbol(const char symbol, const Location& location,
                                   List<char> expected)
    : UnexpectedSymbol(symbol, location) {
  String expected_str;
  if (expected.size() == 1) {
    expected_str = "symbol " + String(1, expected.front());
  } else {
    expected_str = "one of symbols (single space separated): ";
    for (const char s : expected) {
      // FIXME: print in a more friendly way.
      expected_str += String(1, s) + " ";
    }
  }
  SetExpected(expected_str);
}

UnexpectedToken::UnexpectedToken(const Token& token)
    : SyntaxError(token.location()) {
  SetUnexpected("token " + token.value());
}

UnexpectedToken::UnexpectedToken(const Token& token,
                                 const Token::TypeList& expected)
    : UnexpectedToken(token) {
  String expected_str;
  if (expected.size() == 1) {
    expected_str = "token type: " + Token::PrintType(expected.front());
  } else {
    expected_str = "one of token types: ";
    for (const auto& type : expected) {
      expected_str += Token::PrintType(type) + " ";
    }
  }
  SetExpected(expected_str);
}

UnexpectedEndOfTokens::UnexpectedEndOfTokens(
    const Token::TypeList& expected_types, const Location& location)
    : SyntaxError(location) {
  // TODO: compose a proper string.
  (void)expected_types;
  SetExpected("");
}

}  // namespace shinobi::language::shi
