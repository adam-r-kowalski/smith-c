#define SMITH_ENABLE_ALLOCATOR_MACROS

#include "smith/assertions.h"
#include "smith/hash_interner.h"
#include "smith/random.h"
#include "smith/string.h"
#include "smith/system_allocator.h"
#include "smith/test_suites.h"
#include "smith/tokenizer.h"

static MunitResult test_smith_tokenize_symbol(const MunitParameter params[],
                                              void *user_data_or_fixture) {
  smith_allocator_t allocator = smith_system_allocator_create();
  smith_interner_t interner = smith_hash_interner_create(allocator);
  smith_string_t symbol = smith_random_symbol(allocator);
  smith_intern_result_t intern_result = smith_interner_intern(interner, symbol);
  munit_assert(intern_result.success);
  smith_cursor_t cursor = {.source = symbol.data};
  smith_next_token_result_t actual = smith_next_token(interner, cursor);
  smith_position_t end = {.column = symbol.length};
  smith_next_token_result_t expected = {
      .token = {.kind = SMITH_TOKEN_KIND_SYMBOL,
                .value.symbol = {.interned = intern_result.interned,
                                 .span.end = end}},
      .cursor = {.source = "", .position = end}};
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
