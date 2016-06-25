#pragma once

#include <base/aliases.hh>
#include <base/logging.hh>

#include STL(iostream)
#include STL(sstream)

namespace shinobi {
void GetStackTrace(ui8 depth, Vector<String>& strings);
}  // namespace shinobi

#define CHECK(expr)                                    \
  if (!(expr))                                         \
  [] {                                                 \
    using namespace shinobi;                           \
    Vector<String> strings;                            \
    Log log(named_levels::ASSERT);                     \
    GetStackTrace(62, strings);                        \
    log << "Assertion failed: " << #expr << std::endl; \
    for (size_t i = 1; i < strings.size(); ++i) {      \
      log << "  " << strings[i] << std::endl;          \
    }                                                  \
    return log;                                        \
  }()

// There is a trick how to use lambda in expression:
//
//   DCHECK([&]{ return false; }());
//                              ^^
//
#if defined(NDEBUG)
// TODO: investigate why clang can't link with |__builtin_assume()|.
#define DCHECK_O_EVAL(expr) (void)(expr);
#define DCHECK(expr)
#define NOTREACHED() __builtin_unreachable()
#else
#define DCHECK_O_EVAL(expr) CHECK(expr)
#define DCHECK(expr) CHECK(expr)
#define NOTREACHED() DCHECK(false) << "NOTREACHED"
#endif
