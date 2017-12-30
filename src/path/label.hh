#pragma once

namespace shinobi {

// Labels look like <path>:<target_name>

class Label : public Path {
 public:
  Label(const String& path, const String& target_name)
      : Path(path), name_(target_name) {}
};

}  // namespace shinobi
