#define SMITH_ENABLE_ALLOCATOR_MACROS

#include "smith/random.h"
#include <munit.h>

#define MIN_LENGTH 1
#define MAX_LENGTH 10

smith_string_t smith_random_string(smith_allocator_t allocator) {
  size_t length = munit_rand_int_range(MIN_LENGTH, MAX_LENGTH);
  char *data = smith_allocator_allocate_array(allocator, char, length + 1);
  munit_assert(data != nullptr);
  for (size_t i = 0; i < length; i++) {
    data[i] = munit_rand_int_range(0x20, 0x7E);
  }
  data[length] = '\0';
  return (smith_string_t){.data = data, .length = length};
}

smith_string_t smith_random_symbol(smith_allocator_t allocator) {
  size_t length = munit_rand_int_range(MIN_LENGTH, MAX_LENGTH);
  char *data = smith_allocator_allocate_array(allocator, char, length + 1);
  munit_assert(data != nullptr);
  for (size_t i = 0; i < length; i++) {
    switch (munit_rand_int_range(0, 3)) {
    case 0:
      data[i] = munit_rand_int_range('a', 'z');
      break;
    case 1:
      data[i] = munit_rand_int_range('A', 'Z');
      break;
    case 2:
      data[i] = i > 0 ? munit_rand_int_range('0', '9') : '_';
      break;
    default:
      data[i] = '_';
      break;
    }
  }
  data[length] = '\0';
  return (smith_string_t){.data = data, .length = length};
}

smith_string_t smith_random_int(smith_allocator_t allocator) {
  size_t length = munit_rand_int_range(MIN_LENGTH, MAX_LENGTH);
  char *data = smith_allocator_allocate_array(allocator, char, length + 1);
  munit_assert(data != nullptr);
  for (size_t i = 0; i < length; i++) {
    data[i] = munit_rand_int_range('0', '9');
  }
  data[length] = '\0';
  return (smith_string_t){.data = data, .length = length};
}
