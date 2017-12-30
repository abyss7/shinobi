#pragma once

#include <base/aliases.hh>
#include <language/shi/node.hh>
#include <target.hh>

namespace shinobi::language {

/* Semantic Tree.

      Node types of the tree are:

        Variable (String, Integer) - single typed value.
            - Must be assigned.
            - Must be referenced.

        List (String) - list of string values.
            - May be empty.

        Target - typed set of lists and/or variables.
            - Type may be built-in or generated.
            - May have string name.
 */

class Sema {
 public:
  virtual ~Sema() {}

  virtual List<Target> Process(NodePtr top_node) = 0;
};

}  // namespace shinobi::language
