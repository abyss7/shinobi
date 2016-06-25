#pragma once

#include <base/aliases.hh>
#include <base/path.hh>

namespace shinobi {

class Location {
 public:
  Location() {}
  Location(const Path& file_path, ui64 line, ui64 column);

  const Path& file_path() const { return path_; }
  const ui64 line() const { return line_; }
  const ui64 column() const { return column_; }

  operator bool() const;
  bool operator<(const Location& other) const;

 private:
  static constexpr ui64 INVALID_VALUE = 0u;

  const Path path_;
  const ui64 line_ = INVALID_VALUE, column_ = INVALID_VALUE;
};

class LocationRange {
 public:
  LocationRange(const Location& begin, const Location& end);
  // The end is exclusive, i.e. [begin, end)

  const Location& begin() const { return begin_; }
  const Location& end() const { return end_; }

 private:
  const Location begin_, end_;
};

}  // namespace shinobi
