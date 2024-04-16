#include "smith/assertions.h"
#include "smith/hash_interner.h"
#include "smith/parser.h"
#include "smith/random.h"
#include "smith/system_allocator.h"
#include "smith/test_suites.h"

static smith_interner_t interner_create(smith_allocator_t allocator) {
  smith_hash_interner_create_result_t interner_create_result =
      smith_hash_interner_create(allocator);
  munit_assert(interner_create_result.success);
  return interner_create_result.interner;
}

static smith_keywords_t keywords_create(smith_interner_t interner) {
  smith_keywords_create_result_t keywords_create_result =
      smith_keywords_create(interner);
  munit_assert(keywords_create_result.success);
  return keywords_create_result.keywords;
}

static smith_interned_t intern(smith_interner_t interner,
                               smith_string_t string) {
  smith_intern_result_t intern_result = smith_interner_intern(interner, string);
  munit_assert(intern_result.success);
  return intern_result.interned;
}

static MunitResult test_smith_parse_symbol(const MunitParameter params[],
                                           void *user_data_or_fixture) {
  smith_allocator_t allocator = smith_system_allocator_create();
  smith_interner_t interner = interner_create(allocator);
  smith_keywords_t keywords = keywords_create(interner);
  smith_string_t symbol = smith_random_symbol(allocator);
  smith_interned_t interned = intern(interner, symbol);
  smith_cursor_t cursor = {.source = symbol.data};
  smith_parse_result_t actual =
      smith_parse_expression(allocator, interner, cursor, keywords);
  smith_position_t end = {.column = symbol.length};
  smith_parse_result_t expected = {
      .expression = {.kind = SMITH_EXPRESSION_KIND_SYMBOL,
                     .value.symbol = {.interned = interned, .span.end = end}},
      .cursor = {.source = "", .position = end}};
  smith_assert_parse_result_equal(actual, expected);
  smith_interner_destroy(interner);
  smith_allocator_destroy(allocator);
  return MUNIT_OK;
}

static MunitResult test_smith_parse_int(const MunitParameter params[],
                                        void *user_data_or_fixture) {
  smith_allocator_t allocator = smith_system_allocator_create();
  smith_interner_t interner = interner_create(allocator);
  smith_keywords_t keywords = keywords_create(interner);
  smith_string_t int_ = smith_random_int(allocator);
  smith_interned_t interned = intern(interner, int_);
  smith_cursor_t cursor = {.source = int_.data};
  smith_parse_result_t actual =
      smith_parse_expression(allocator, interner, cursor, keywords);
  smith_position_t end = {.column = int_.length};
  smith_parse_result_t expected = {
      .expression = {.kind = SMITH_EXPRESSION_KIND_INT,
                     .value.int_ = {.interned = interned, .span.end = end}},
      .cursor = {.source = "", .position = end}};
  smith_assert_parse_result_equal(actual, expected);
  smith_interner_destroy(interner);
  smith_allocator_destroy(allocator);
  return MUNIT_OK;
}

static MunitTest smith_parser_tests[] = {{
                                             .name = "/test_smith_parse_symbol",
                                             .test = test_smith_parse_symbol,
                                         },
                                         {
                                             .name = "/test_smith_parse_int",
                                             .test = test_smith_parse_int,
                                         },
                                         {}};

MunitSuite smith_parser_suite = {
    .prefix = "/parser",
    .tests = smith_parser_tests,
    .iterations = 1,
};
