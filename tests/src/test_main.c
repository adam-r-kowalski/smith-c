#include "smith/test_suites.h"
#include <munit.h>

int32_t main(int argc, char *argv[]) {
  MunitSuite suites[] = {smith_tokenizer_suite, {}};

  MunitSuite main_suite = {.prefix = "All Tests",
                           .suites = suites,
                           .iterations = 1,
                           .options = MUNIT_SUITE_OPTION_NONE};

  return munit_suite_main(&main_suite, nullptr, argc, argv);
}
