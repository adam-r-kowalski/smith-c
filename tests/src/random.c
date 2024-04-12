#define SMITH_ENABLE_ALLOCATOR_MACROS

#include "smith/random.h"
#include <munit.h>

int32_t smith_random_uniform(int32_t min, int32_t max) {
  return munit_rand_int_range(min, max);
}

smith_string_t smith_random_string(smith_allocator_t allocator,
                                   int32_t length) {
  char *data = smith_allocator_allocate_array(allocator, char, length + 1);
  munit_assert(data != nullptr);
  for (size_t i = 0; i < length; i++) {
    data[i] = munit_rand_int_range(0x20, 0x7E);
  }
  data[length] = '\0';
  return (smith_string_t){.data = data, .length = length};
}
