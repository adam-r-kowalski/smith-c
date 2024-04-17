#include "smith/assertions.h"
#include "smith/format.h"
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

static smith_parser_context_t parser_context_create() {
  smith_allocator_t allocator = smith_system_allocator_create();
  smith_interner_t interner = interner_create(allocator);
  smith_keywords_t keywords = keywords_create(interner);
  return (smith_parser_context_t){
      .allocator = allocator, .interner = interner, .keywords = keywords};
}

void parser_context_destroy(smith_parser_context_t context) {
  smith_interner_destroy(context.interner);
  smith_allocator_destroy(context.allocator);
}

static MunitResult test_smith_parse_symbol(const MunitParameter params[],
                                           void *user_data_or_fixture) {
  smith_parser_context_t context = parser_context_create();
  smith_string_t symbol = smith_random_symbol(context.allocator);
  smith_cursor_t cursor = {.source = symbol.data};
  smith_parse_result_t actual = smith_parse_expression(context, cursor);
  smith_position_t end = {.column = symbol.length};
  smith_interned_t interned = intern(context.interner, symbol);
  smith_parse_result_t expected = {
      .expression = {.kind = SMITH_EXPRESSION_KIND_SYMBOL,
                     .value.symbol = {.interned = interned, .span.end = end}},
      .cursor = {.source = "", .position = end}};
  smith_assert_parse_result_equal(actual, expected);
  parser_context_destroy(context);
  return MUNIT_OK;
}

static MunitResult test_smith_parse_int(const MunitParameter params[],
                                        void *user_data_or_fixture) {
  smith_parser_context_t context = parser_context_create();
  smith_string_t int_ = smith_random_int(context.allocator);
  smith_cursor_t cursor = {.source = int_.data};
  smith_parse_result_t actual = smith_parse_expression(context, cursor);
  smith_position_t end = {.column = int_.length};
  smith_interned_t interned = intern(context.interner, int_);
  smith_parse_result_t expected = {
      .expression = {.kind = SMITH_EXPRESSION_KIND_INT,
                     .value.int_ = {.interned = interned, .span.end = end}},
      .cursor = {.source = "", .position = end}};
  smith_assert_parse_result_equal(actual, expected);
  parser_context_destroy(context);
  return MUNIT_OK;
}

static MunitResult test_smith_parse_float(const MunitParameter params[],
                                          void *user_data_or_fixture) {
  smith_parser_context_t context = parser_context_create();
  smith_string_t float_ = smith_random_int(context.allocator);
  smith_cursor_t cursor = {.source = float_.data};
  smith_parse_result_t actual = smith_parse_expression(context, cursor);
  smith_position_t end = {.column = float_.length};
  smith_interned_t interned = intern(context.interner, float_);
  smith_parse_result_t expected = {
      .expression = {.kind = SMITH_EXPRESSION_KIND_INT,
                     .value.float_ = {.interned = interned, .span.end = end}},
      .cursor = {.source = "", .position = end}};
  smith_assert_parse_result_equal(actual, expected);
  parser_context_destroy(context);
  return MUNIT_OK;
}

static MunitResult
test_smith_parse_binary_operator(const MunitParameter params[],
                                 void *user_data_or_fixture) {
  smith_parser_context_t context = parser_context_create();
  smith_string_t a = smith_random_float(context.allocator);
  smith_string_t b = smith_random_float(context.allocator);
  char *source =
      smith_format_string(context.allocator, "%s + %s", a.data, b.data);
  munit_assert_not_null(source);
  smith_cursor_t cursor = {.source = source};
  smith_parse_result_t actual = smith_parse_expression(context, cursor);
  smith_interned_t a_interned = intern(context.interner, a);
  smith_interned_t b_interned = intern(context.interner, b);
  smith_position_t a_end = {.column = a.length};
  smith_position_t b_start = {.column = a.length + 3};
  smith_span_t b_span = {.start = b_start,
                         .end.column = b_start.column + b.length};
  smith_parse_result_t expected = {
      .expression = {.kind = SMITH_EXPRESSION_KIND_BINARY_OPERATOR,
                     .value.binary_operator =
                         {.kind = SMITH_BINARY_OPERATOR_KIND_ADD,
                          .left =
                              &(smith_expression_t){
                                  .kind = SMITH_EXPRESSION_KIND_FLOAT,
                                  .value.float_ = {.interned = a_interned,
                                                   .span.end = a_end}},
                          .right =
                              &(smith_expression_t){
                                  .kind = SMITH_EXPRESSION_KIND_FLOAT,
                                  .value.float_ = {.interned = b_interned,
                                                   .span = b_span}}}},
      .cursor = {.source = "", .position = b_span.end}};
  smith_assert_parse_result_equal(actual, expected);
  smith_expression_destroy(context.allocator, actual.expression);
  parser_context_destroy(context);
  return MUNIT_OK;
}

static MunitTest smith_parser_tests[] = {
    {
        .name = "/test_smith_parse_symbol",
        .test = test_smith_parse_symbol,
    },
    {
        .name = "/test_smith_parse_int",
        .test = test_smith_parse_int,
    },
    {
        .name = "/test_smith_parse_float",
        .test = test_smith_parse_float,
    },
    {
        .name = "/test_smith_parse_binary_operator",
        .test = test_smith_parse_binary_operator,
    },
    {}};

MunitSuite smith_parser_suite = {
    .prefix = "/parser",
    .tests = smith_parser_tests,
    .iterations = 1,
};
