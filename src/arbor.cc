#include <arbor.hh>

namespace shinobi {

void Arbor::Parse(const Label& path) {
  unparsed_.Push(path);
}

}  // namespace shinobi
