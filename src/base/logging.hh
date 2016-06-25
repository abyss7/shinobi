#pragma once

#include <base/aliases.hh>

#include STL(set)
#include STL(sstream)

namespace shinobi {

// Since the enum values are defined in the parent's scope - to be able to use
// something like this:
//
//     LOG(ERROR) << "Message";
//
// in any part of code, we need a separate namespace with an enum only, to
// include it with "using namespace NamedLevels;". See using_log.h file.

namespace named_levels {

#pragma push_macro("FATAL")
#pragma push_macro("ASSERT")
#pragma push_macro("ERROR")
#pragma push_macro("WARNING")
#pragma push_macro("INFO")
#pragma push_macro("VERBOSE")
#pragma push_macro("TRACE")

#undef FATAL
#undef ASSERT

#undef ERROR
#undef WARNING
#undef INFO
#undef VERBOSE
#undef TRACE

enum : ui32 {
  FATAL = 0u,
  // after |LOG(FATAL)| the program terminates with |exit(1)|.

  ASSERT = 1u,
  // used only inside |CHECK()|, can't be filtered by verbosity levels.

  ERROR = 10u,
  WARNING = 20u,
  INFO = 30u,
  VERBOSE = 40u,
  TRACE = 50u,
};

#pragma pop_macro("FATAL")
#pragma pop_macro("ASSERT")
#pragma pop_macro("ERROR")
#pragma pop_macro("WARNING")
#pragma pop_macro("INFO")
#pragma pop_macro("VERBOSE")
#pragma pop_macro("TRACE")

}  // namespace NamedLevels

class Log {
 public:
  // First value is a right edge of interval, the second - a left edge.
  using RangeSet = std::set<Pair<ui32>>;

  enum Mode {
    CONSOLE,
    SYSLOG,
  };

  // We need a separate method to be able to change mode before daemonizing.
  static void SetMode(Mode mode);

  // Expects, that ranges are already filtered.
  static void Reset(ui32 error_mark, RangeSet&& ranges);

  Log(ui32 level);
  ~Log();

  Log(Log&&) = default;
  Log(const Log&) = delete;
  Log& operator=(const Log&) = delete;

  template <class T>
  Log& operator<<(const T& info) {
    stream_ << info;
    return *this;
  }

  template <class T>
  Log& operator<<(const List<T>& info) {
    auto it = info.begin();
    this->operator<<(*it);
    ++it;
    for (; it != info.end(); ++it) {
      stream_ << " ";
      this->operator<<(*it);
    }
    return *this;
  }

  Log& operator<<(std::ostream& (*func)(std::ostream&));  // for |std::endl|

 private:
  static ui32& error_mark();
  static SharedPtr<RangeSet>& ranges();
  static Mode& mode();

  ui32 level_;
  ui32 error_mark_;
  SharedPtr<RangeSet> ranges_;
  std::stringstream stream_;
  Mode mode_ = CONSOLE;
};

}  // namespace dist_clang
