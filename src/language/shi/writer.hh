#pragma once

#include <base/aliases.hh>
#include <language/shi/node.hh>

namespace shinobi::language::shi {

class Writer {
 public:
  struct Configuration {
    ui8 indentation = 2;
  };

  explicit Writer(const Configuration& config);

  String Write(const NodePtr& top_node) const;

 private:
  void Indent(String& output, ui8 nesting) const;
  void NewLine(String& output) const;

  void PrintAssignment(String& output, const NodePtr& node, ui8 nesting) const;
  void PrintCall(String& output, const NodePtr& node, ui8 nesting) const;
  void PrintCondition(String& output, const NodePtr& node, ui8 nesting) const;
  void PrintStatementList(String& output, const NodePtr& node,
                          ui8 nesting) const;

  const Configuration conf_;
};

}  // namespace shinobi::language::shi
