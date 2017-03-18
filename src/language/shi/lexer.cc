#include <language/shi/lexer.hh>

#include <base/assert.hh>
#include <language/shi/exception.hh>

namespace shinobi::language::shi {

Lexer::Lexer(const Path& file_path, const String& contents)
    : path_(file_path), contents_(contents) {}

Vector<Token> Lexer::Tokenize() {
  Vector<Token> tokens;

  bool done = false;
  while (!done) {
    tokens.emplace_back(Next(done));
  }

  return tokens;
}

Token Lexer::Next(bool& done) {
  // Check that current position is inside contents.
  if (current_ == contents_.size()) {
    done = true;
    return Token();
  }
  DCHECK(current_ < contents_.size());

  // Trim all the whitespace before token.
  for (bool token_found = false; current_ < contents_.size() && !token_found;) {
    switch (Current()) {
      case ' ':
      case '\t':
      case '\r': {
        Advance();
        continue;
      }
      case '\n': {
        NewLine();
        continue;
      }
      default:
        token_found = true;
    }
  }

  auto location = CurrentLocation();

  // Consume the token.
  Token::Type token_type;
  if (IsOneSymbolToken(token_type)) {
    Advance();
    return Token(location, token_type);
  } else if (IsOneOrTwoSymbolsToken(token_type)) {
    if (LookAhead() == '=') {
      Advance(2);
      switch (token_type) {
        case Token::PLUS:
          return Token(location, Token::PLUS_EQUALS);
        case Token::MINUS:
          return Token(location, Token::MINUS_EQUALS);
        case Token::BANG:
          return Token(location, Token::NOT_EQUAL);
        case Token::EQUAL:
          return Token(location, Token::EQUAL_EQUAL);
        case Token::STRICTLY_LESS:
          return Token(location, Token::LESS_EQUAL);
        case Token::STRICTLY_GREATER:
          return Token(location, Token::GREATER_EQUAL);
        default:
          NOTREACHED();
      }
    } else {
      Advance();
      return Token(location, token_type);
    }
  } else if (IsTwoSymbolsToken(token_type)) {
    Advance(2);
    return Token(location, token_type);
  } else if (IsInteger()) {
    return ConsumeInteger();
  } else if (IsString()) {
    return ConsumeString();
  } else if (IsIdentifierOrKeyword()) {
    return ConsumeIdentifierOrKeyword();
  } else if (IsComment()) {
    return ConsumeComment();
  }

  throw UnexpectedSymbol(Current(), CurrentLocation());

  NOTREACHED();
  return Token();
}

char Lexer::LookAhead() const {
  DCHECK(current_ < contents_.size());
  if (current_ + 1 == contents_.size()) {
    return '\0';
  }

  return contents_[current_ + 1];
}

void Lexer::Advance(ui64 step) {
  column_ += step;
  current_ += step;
}

void Lexer::NewLine() {
  ++line_;
  column_ = 1;
  ++current_;
}

bool Lexer::IsOneSymbolToken(Token::Type& type) const {
  switch (Current()) {
    case '{':
      type = Token::LEFT_BRACE;
      return true;
    case '}':
      type = Token::RIGHT_BRACE;
      return true;
    case '(':
      type = Token::LEFT_PAREN;
      return true;
    case ')':
      type = Token::RIGHT_PAREN;
      return true;
    case '[':
      type = Token::LEFT_BRACE;
      return true;
    case ']':
      type = Token::RIGHT_BRACE;
      return true;
    case '.':
      type = Token::DOT;
      return true;
    case ',':
      type = Token::COMMA;
      return true;
  }

  return false;
}

bool Lexer::IsOneOrTwoSymbolsToken(Token::Type& type) const {
  switch (Current()) {
    case '+':
      type = Token::PLUS;
      return true;
    case '-':
      type = Token::MINUS;
      return true;
    case '!':
      type = Token::BANG;
      return true;
    case '=':
      type = Token::EQUAL;
      return true;
    case '<':
      type = Token::STRICTLY_LESS;
      return true;
    case '>':
      type = Token::STRICTLY_GREATER;
      return true;
  }

  return false;
}

bool Lexer::IsTwoSymbolsToken(Token::Type& type) const {
  switch (Current()) {
    case '&': {
      if (LookAhead() == '&') {
        type = Token::BOOLEAN_AND;
        return true;
      }
    } break;
    case '|': {
      if (LookAhead() == '|') {
        type = Token::BOOLEAN_OR;
        return true;
      }
    } break;
  }

  return false;
}

bool Lexer::IsInteger() const {
  return Current() >= '0' && Current() <= '9';
}

bool Lexer::IsString() const {
  return Current() == '"';
}

bool Lexer::IsIdentifierOrKeyword() const {
  return (Current() >= 'A' && Current() <= 'Z') ||
         (Current() >= 'a' && Current() <= 'z') || Current() == '_';
}

bool Lexer::IsComment() const {
  return Current() == '#';
}

Token Lexer::ConsumeInteger() {
  auto location = CurrentLocation();
  while (current_ < contents_.size() && IsInteger()) {
    Advance();
  }

  return Token(
      location, Token::INTEGER,
      contents_.substr(location.column(),
                       CurrentLocation().column() - location.column()));
}

Token Lexer::ConsumeString() {
  auto location = CurrentLocation();
  Advance();
  while (current_ < contents_.size() && Current() != '"' && Current() != '\n') {
    Advance();
  }

  if (Current() != '"') {
    throw UnexpectedSymbol(Current(), CurrentLocation());
  }
  Advance();

  return Token(
      location, Token::STRING,
      contents_.substr(location.column(),
                       CurrentLocation().column() - location.column()));
}

Token Lexer::ConsumeIdentifierOrKeyword() {
  auto location = CurrentLocation();
  while (current_ < contents_.size() &&
         (IsIdentifierOrKeyword() || IsInteger())) {
    Advance();
  }

  auto value = contents_.substr(location.column(),
                                CurrentLocation().column() - location.column());

  if (value == "if") {
    return Token(location, Token::IF_TOKEN);
  } else if (value == "else") {
    return Token(location, Token::ELSE_TOKEN);
  } else if (value == "true") {
    return Token(location, Token::TRUE_TOKEN);
  } else if (value == "false") {
    return Token(location, Token::FALSE_TOKEN);
  }

  return Token(location, Token::IDENTIFIER, value);
}

Token Lexer::ConsumeComment() {
  auto location = CurrentLocation();
  while (current_ < contents_.size() && Current() != '\n') {
    Advance();
  }

  return Token(
      location, Token::COMMENT,
      contents_.substr(location.column(),
                       CurrentLocation().column() - location.column()));
}

}  // namespace shinobi::language::shi
