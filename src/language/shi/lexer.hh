#pragma once

#include <language/shi/token.hh>

namespace shinobi::language::shi {

class Lexer {
 public:
  Lexer(const Path& file_path, const String& contents);

  Vector<Token> Tokenize();

 private:
  Token Next(bool& done);

  Location CurrentLocation() const { return Location(path_, line_, column_); }
  const char& Current() const { return contents_[current_]; }
  char LookAhead() const;
  // returns \0 if there is nothing ahead.

  void Advance(ui64 step = 1);
  void NewLine();

  bool IsOneSymbolToken(Token::Type& type) const;
  bool IsOneOrTwoSymbolsToken(Token::Type& type) const;
  bool IsTwoSymbolsToken(Token::Type& type) const;
  bool IsInteger() const;
  bool IsString() const;
  bool IsIdentifierOrKeyword() const;
  bool IsComment() const;

  Token ConsumeInteger();
  Token ConsumeString();
  Token ConsumeIdentifierOrKeyword();
  Token ConsumeComment();

  const Path& path_;
  const String& contents_;
  ui64 current_ = 0, line_ = 1, column_ = 1;
};

}  // namespace shinobi::language::shi
