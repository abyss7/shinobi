#include <base/assert.hh>

#include STL(cxxabi.h)

#include <execinfo.h>

namespace shinobi {

namespace {

String Demangle(const char* backtrace_symbol) {
  const String string = backtrace_symbol;

  auto begin_name = string.find('(');
  if (begin_name == String::npos) {
    return string;
  }
  begin_name++;

  const auto end_name = string.find('+', begin_name);
  if (end_name == String::npos) {
    return string;
  }

  const String mangled_name = string.substr(begin_name, end_name - begin_name);
  size_t size = 256;
  int status;
  char* demangled_name =
      abi::__cxa_demangle(mangled_name.c_str(), nullptr, &size, &status);
  if (status == 0) {
    auto result = String(demangled_name);
    free(demangled_name);
    return result;
  } else {
    if (demangled_name) {
      free(demangled_name);
    }
    return mangled_name;
  }
}

}  // namespace

void GetStackTrace(ui8 depth, Vector<String>& strings) {
  using void_ptr = void*;
  UniquePtr<void_ptr[]> buffer(new void_ptr[depth + 1]);

  auto size = backtrace(buffer.get(), depth + 1);
  auto symbols = backtrace_symbols(buffer.get(), size);

  // Two last frames always look like:
  //     backtrace
  //     shinobi::GetStackTrace(...)
  //
  // We don't need them.

  strings.resize(size - 2);
  for (int i = 2; i < size; ++i) {
    strings[i - 2] = Demangle(symbols[i]);
  }
  free(symbols);
}

}  // namespace shinobi
