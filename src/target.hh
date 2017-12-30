#pragma once

namespace shinobi {

class Path;

class File : public Path {};
class Label : public Path {};

class Target {
 public:
  virtual ~Target();

  Label Name() const;

// TODO: |List| is a sorted list - almost like a |std::set|.
#define PROPERTY(name, type)             \
  void add_##name(const List<type>&);    \
  void remove_##name(const List<type>&); \
  const List<type>& name() const;

  // Source interface:
  //   |add_*| throw exception if some of the files are already there.
  //   |remote_*| throw exception if some of the files or labels not found.

  // Sources are subject for rules to be applied on them, also sources can
  // be parsed in some known way to check real code dependencies.
  PROPERTY(sources, File)

  // Public are supplement to the sources. Public sources may be referenced
  // by sources of direct dependent targets.
  PROPERTY(public, File)

  // Inputs affect target in a way that if they change the target must be
  // rebuilt - excluding sources.
  PROPERTY(inputs, File)

  // Run-time data file dependencies.
  PROPERTY(data, File)

  // All files produced from sources and inputs. Can be used as inputs or
  // sources for other targets, if the producing target is mentioned in deps.
  PROPERTY(outputs, File)

  // Build-time deps directly required to get some outputs from target:
  // actually it's deps' outputs that are required - not the targets it-selves.
  PROPERTY(deps, Label)

  // Public deps are supplement to the deps.
  PROPERTY(public_deps, Label)

  // Run-time deps are required for proper execution, e.g. in tests.
  PROPERTY(data_deps, Label)

#undef PROPERTY

 protected:
  Target(const Label& name);
};

}  // namespace shinobi
