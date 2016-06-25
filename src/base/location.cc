#include <base/location.hh>

#include <base/assert.hh>

namespace shinobi {

Location::Location(const Path& file_path, ui64 line_number, ui64 column_number)
    : path_(file_path), line_(line_number), column_(column_number) {
  DCHECK(line_ != INVALID_VALUE);
  DCHECK(column_ != INVALID_VALUE);
}

Location::operator bool() const {
  return line_ != INVALID_VALUE && column_ != INVALID_VALUE;
}

bool Location::operator<(const Location& other) const {
  DCHECK(path_ == other.path_);
  return std::tie(line_, column_) < std::tie(other.line_, other.column_);
}

LocationRange::LocationRange(const Location& begin, const Location& end)
    : begin_(begin), end_(end) {
  DCHECK(begin_ < end_);
}

}  // namespace shinobi
