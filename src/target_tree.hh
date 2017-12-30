#pragma once

namespace shinobi {

class TargetTree {
 public:
  Compose(std::unique_ptr<Target> target);

 private:
  List<Target*> roots_;
  Trie<std::unique_ptr<Target>> targets_;
};

}  // namespace shinobi
