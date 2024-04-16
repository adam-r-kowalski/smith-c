#define SMITH_ENABLE_ALLOCATOR_MACROS

#include "smith/finite_allocator.h"
#include "smith/hash_interner.h"
#include "smith/random.h"
#include "smith/string.h"
#include "smith/system_allocator.h"
#include "smith/test_suites.h"

static smith_interner_t interner_create(smith_allocator_t allocator) {
  smith_hash_interner_create_result_t interner_create_result =
      smith_hash_interner_create(allocator);
  munit_assert(interner_create_result.success);
  return interner_create_result.interner;
}

static smith_allocator_t finite_allocator_create(smith_allocator_t parent,
                                                 size_t allocations) {
  smith_finite_allocator_create_result_t finite_allocator_create_result =
      smith_finite_allocator_create(parent, allocations);
  munit_assert(finite_allocator_create_result.success);
  return finite_allocator_create_result.allocator;
}

static smith_interned_t intern(smith_interner_t interner,
                               smith_string_t string) {
  smith_intern_result_t intern_result = smith_interner_intern(interner, string);
  munit_assert(intern_result.success);
  return intern_result.interned;
}

static smith_string_t lookup(smith_interner_t interner,
                             smith_interned_t interned) {
  smith_lookup_result_t lookup_result =
      smith_interner_lookup(interner, interned);
  munit_assert(lookup_result.success);
  return lookup_result.string;
}

static MunitResult test_smith_intern_and_lookup(const MunitParameter params[],
                                                void *user_data_or_fixture) {
  smith_allocator_t allocator = smith_system_allocator_create();
  smith_interner_t interner = interner_create(allocator);
  smith_string_t string = smith_random_string(allocator);
  smith_interned_t interned = intern(interner, string);
  smith_string_t lookedup = lookup(interner, interned);
  munit_assert_string_equal(string.data, lookedup.data);
  smith_interner_destroy(interner);
  smith_allocator_deallocate(allocator, string.data);
  smith_allocator_destroy(allocator);
  return MUNIT_OK;
}

static MunitResult
test_smith_interner_allocation_failure(const MunitParameter params[],
                                       void *user_data_or_fixture) {
  smith_allocator_t allocator =
      finite_allocator_create(smith_system_allocator_create(), 2);
  smith_interner_t interner = interner_create(allocator);
  smith_string_t string = smith_random_string(allocator);
  smith_intern_result_t intern_result = smith_interner_intern(interner, string);
  munit_assert(!intern_result.success);
  smith_interner_destroy(interner);
  smith_allocator_deallocate(allocator, string.data);
  smith_allocator_destroy(allocator);
  return MUNIT_OK;
}

static MunitResult test_smith_interner_grows(const MunitParameter params[],
                                             void *user_data_or_fixture) {
  smith_allocator_t allocator = smith_system_allocator_create();
  smith_interner_t interner = interner_create(allocator);
  smith_string_t strings[SMITH_HASH_INTERNER_MIN_CAPACITY + 1];
  smith_interned_t interneds[SMITH_HASH_INTERNER_MIN_CAPACITY + 1];
  for (size_t i = 0; i < SMITH_HASH_INTERNER_MIN_CAPACITY + 1; i++) {
    strings[i] = smith_random_string(allocator);
    smith_intern_result_t intern_result =
        smith_interner_intern(interner, strings[i]);
    munit_assert(intern_result.success);
    interneds[i] = intern_result.interned;
  }
  for (size_t i = 0; i < SMITH_HASH_INTERNER_MIN_CAPACITY + 1; i++) {
    smith_lookup_result_t lookup_result =
        smith_interner_lookup(interner, interneds[i]);
    munit_assert(lookup_result.success);
    munit_assert_string_equal(strings[i].data, lookup_result.string.data);
  }
  for (size_t i = 0; i < SMITH_HASH_INTERNER_MIN_CAPACITY + 1; i++) {
    smith_allocator_deallocate(allocator, strings[i].data);
  }
  smith_interner_destroy(interner);
  smith_allocator_destroy(allocator);
  return MUNIT_OK;
}

static MunitTest smith_hash_interner_tests[] = {
    {
        .name = "/test_smith_intern_and_lookup",
        .test = test_smith_intern_and_lookup,
    },
    {
        .name = "/test_smith_interner_allocation_failure",
        .test = test_smith_interner_allocation_failure,
    },
    {
        .name = "/test_smith_interner_grows",
        .test = test_smith_interner_grows,
    },
    {}};

MunitSuite smith_hash_interner_suite = {
    .prefix = "/hash_interner",
    .tests = smith_hash_interner_tests,
    .iterations = 1,
};
