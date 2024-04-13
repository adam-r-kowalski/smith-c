#include "smith/assertions.h"
#include <munit.h>

void smith_assert_position_equal(smith_position_t actual,
                                 smith_position_t expected) {
  munit_assert_int(actual.line, ==, expected.line);
  munit_assert_int(actual.column, ==, expected.column);
}

void smith_assert_span_equal(smith_span_t actual, smith_span_t expected) {
  smith_assert_position_equal(actual.start, expected.start);
  smith_assert_position_equal(actual.end, expected.end);
}

void smith_assert_cursor_equal(smith_cursor_t actual, smith_cursor_t expected) {
  smith_assert_position_equal(actual.position, expected.position);
  munit_assert_string_equal(actual.source, expected.source);
}

void smith_assert_symbol_equal(smith_symbol_t actual, smith_symbol_t expected) {
  smith_assert_span_equal(actual.span, expected.span);
  munit_assert_int(actual.interned, ==, expected.interned);
}

void smith_assert_int_equal(smith_int_t actual, smith_int_t expected) {
  smith_assert_span_equal(actual.span, expected.span);
  munit_assert_int(actual.interned, ==, expected.interned);
}

void smith_assert_float_equal(smith_float_t actual, smith_float_t expected) {
  smith_assert_span_equal(actual.span, expected.span);
  munit_assert_int(actual.interned, ==, expected.interned);
}

void smith_assert_token_equal(smith_token_t actual, smith_token_t expected) {
  munit_assert_int(actual.kind, ==, expected.kind);
  switch (actual.kind) {
  case SMITH_TOKEN_KIND_SYMBOL:
    return smith_assert_symbol_equal(actual.value.symbol,
                                     expected.value.symbol);
  case SMITH_TOKEN_KIND_INT:
    return smith_assert_int_equal(actual.value.int_, expected.value.int_);
  case SMITH_TOKEN_KIND_FLOAT:
    return smith_assert_float_equal(actual.value.float_, expected.value.float_);
  default:
    munit_assert(false);
  }
}

void smith_assert_next_token_result_equal(smith_next_token_result_t actual,
                                          smith_next_token_result_t expected) {
  smith_assert_token_equal(actual.token, expected.token);
  smith_assert_cursor_equal(actual.cursor, expected.cursor);
}
