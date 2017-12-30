#pragma once

#include <base/aliases.hh>

namespace shinobi {

class Arbor {
 public:
  using Label = struct { const String path, name; };

  void AddTarget(const Label& path);
  // Add new path to be parsed to the queue.
  // Returns false, if path is already parsed, is pending, or is being parsed.

  void Run();
  // Start the parsing.

 private:
  List<Label> targets_;
};

}  // namespace shinobi
