#include <arbor.hh>
#include <base/utils.hh>

int main(int argc, char* argv[]) {
  using namespace shinobi;

  // TODO: implement |Project| interface and move |GetProjectRoot()| there.
  //       The project should at least have a project name.
  Path source_root = base::GetProjectRoot(base::GetCurrentDir());

  // FIXME: do we need |source_root| inside |Arbor|?
  Arbor arbor;

  for (int i = 1; i < argc; ++i) {
    arbor.AddTarget(argv[i]);
  }

  arbor.Run();

  // TODO: do something with a |tree|.
}
