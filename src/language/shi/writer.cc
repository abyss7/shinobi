#include <language/shi/writer.hh>

#include <base/assert.hh>

namespace shinobi::language::shi {

Writer::Writer(const Configuration& config) : conf_(config) {}

String Writer::Write(const NodePtr& top_node) const {
  String result;  // FIXME: reserve size of original file at least.
  PrintStatementList(result, top_node, 0);
  return result;
}

void Writer::Indent(String& output, ui8 nesting) const {
  output += String(nesting * conf_.indentation, ' ');
}

void Writer::NewLine(String& output) const {
  output += String("\n");
}

void Writer::PrintAssignment(String& /* output */, const NodePtr& /* node */,
                             ui8 /* nesting */) const {
  // TODO: implement this method.
}

void Writer::PrintCall(String& /* output */, const NodePtr& /* node */,
                       ui8 /* nesting */) const {
  // TODO: implement this method.
}

void Writer::PrintCondition(String& /* output */, const NodePtr& /* node */,
                            ui8 /* nesting */) const {
  // TODO: implement this method.
}

void Writer::PrintStatementList(String& output, const NodePtr& node,
                                ui8 nesting) const {
  const auto* stmt_list = node->asStatementList();

  for (const auto& stmt : *stmt_list) {
    switch (stmt->type()) {
      case Node::ASSIGNMENT:
        PrintAssignment(output, stmt, nesting);
        break;
      case Node::CALL:
        PrintCall(output, stmt, nesting);
        break;
      case Node::CONDITION:
        PrintCondition(output, stmt, nesting);
        break;
      default:
        NOTREACHED();
    }
  }
}

}  // namespace shinobi::language::shi
