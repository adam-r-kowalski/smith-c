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

void smith_assert_operator_equal(smith_operator_t actual,
                                 smith_operator_t expected) {
  smith_assert_span_equal(actual.span, expected.span);
  munit_assert_int(actual.kind, ==, expected.kind);
}

void smith_assert_delimiter_equal(smith_delimiter_t actual,
                                  smith_delimiter_t expected) {
  smith_assert_span_equal(actual.span, expected.span);
  munit_assert_int(actual.kind, ==, expected.kind);
}

void smith_assert_end_of_file_equal(smith_end_of_file_t actual,
                                    smith_end_of_file_t expected) {
  smith_assert_span_equal(actual.span, expected.span);
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
  case SMITH_TOKEN_KIND_OPERATOR:
    return smith_assert_operator_equal(actual.value.operator_,
                                       expected.value.operator_);
  case SMITH_TOKEN_KIND_DELIMITER:
    return smith_assert_delimiter_equal(actual.value.delimiter,
                                        expected.value.delimiter);
  case SMITH_TOKEN_KIND_END_OF_FILE:
    return smith_assert_end_of_file_equal(actual.value.end_of_file,
                                          expected.value.end_of_file);
  }
}

void smith_assert_next_token_result_equal(smith_next_token_result_t actual,
                                          smith_next_token_result_t expected) {
  smith_assert_token_equal(actual.token, expected.token);
  smith_assert_cursor_equal(actual.cursor, expected.cursor);
}
