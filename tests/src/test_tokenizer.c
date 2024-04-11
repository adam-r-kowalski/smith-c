#define SMITH_ENABLE_ALLOCATOR_MACROS

#include "smith/assertions.h"
#include "smith/hash_interner.h"
#include "smith/string.h"
#include "smith/system_allocator.h"
#include "smith/test_suites.h"
#include "smith/tokenizer.h"

static smith_string_t random_symbol(smith_allocator_t allocator) {
  size_t length = munit_rand_int_range(1, 100);
  char *data = smith_allocator_allocate_array(allocator, char, length + 1);
  for (size_t i = 0; i < length; i++) {
    data[i] = munit_rand_int_range('a', 'z');
  }
  data[length] = '\0';
  return (smith_string_t){.data = data, .length = length};
}

static MunitResult test_smith_tokenize_symbol(const MunitParameter params[],
                                              void *user_data_or_fixture) {
  smith_allocator_t allocator = smith_system_allocator_create();
  smith_interner_t interner = smith_hash_interner_create(allocator);
  smith_string_t symbol = random_symbol(allocator);
  smith_cursor_t cursor = {.source = symbol.data};
  smith_next_token_result_t actual = smith_next_token(interner, cursor);
  smith_next_token_result_t expected = {
      .token = {.kind = SMITH_TOKEN_KIND_SYMBOL}, .cursor = cursor};
  smith_assert_next_token_result_equal(actual, expected);
  smith_interner_destroy(interner);
  smith_allocator_destroy(allocator);
  return MUNIT_OK;
}

static MunitTest smith_tokenizer_tests[] = {
    {
        .name = "/test_smith_tokenize_symbol",
        .test = test_smith_tokenize_symbol,
    },
    {}};

MunitSuite smith_tokenizer_suite = {
    .prefix = "/tokenizer",
    .tests = smith_tokenizer_tests,
    .iterations = 1,
};
