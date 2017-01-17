#include <base/aliases.hh>
#include <base/logging.hh>

// Third-party
#include <gflags/gflags.h>
#include <gtest/gtest.h>

#include <base/using_log.hh>

namespace shinobi {
DEFINE_string(data, String(), "Path to the external data for tests");
// Declare this flag in unit-test source files - where needed.
}

int main(int argc, char* argv[]) {
  // TODO: setup logging.

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::GTEST_FLAG(death_test_style) = "fast";
  ::testing::GTEST_FLAG(show_internal_stack_frames) = false;

  gflags::ParseCommandLineFlags(&argc, &argv, false);

  if (shinobi::FLAGS_data.empty()) {
    LOG(ERROR) << "No data path specified - some tests may fail!";
  }
  return RUN_ALL_TESTS();
}
