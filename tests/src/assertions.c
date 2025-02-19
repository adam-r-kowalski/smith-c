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

void smith_assert_keyword_equal(smith_keyword_t actual,
                                smith_keyword_t expected) {
  smith_assert_span_equal(actual.span, expected.span);
  munit_assert_int(actual.kind, ==, expected.kind);
}

void smith_assert_end_of_file_equal(smith_end_of_file_t actual,
                                    smith_end_of_file_t expected) {
  smith_assert_span_equal(actual.span, expected.span);
}

void smith_assert_unexpected_character_equal(
    smith_unexpected_character_t actual,
    smith_unexpected_character_t expected) {
  smith_assert_span_equal(actual.span, expected.span);
  munit_assert_char(actual.character, ==, expected.character);
}

void smith_assert_interning_failed_equal(smith_interning_failed_t actual,
                                         smith_interning_failed_t expected) {
  smith_assert_span_equal(actual.span, expected.span);
  munit_assert_int(actual.string.length, ==, expected.string.length);
  munit_assert_string_equal(actual.string.data, expected.string.data);
}

void smith_assert_error_equal(smith_error_t actual, smith_error_t expected) {
  munit_assert_int(actual.kind, ==, expected.kind);
  switch (actual.kind) {
  case SMITH_ERROR_KIND_UNEXPECTED_CHARACTER:
    return smith_assert_unexpected_character_equal(
        actual.value.unexpected_character, expected.value.unexpected_character);
  case SMITH_ERROR_KIND_INTERNING_FAILED:
    return smith_assert_interning_failed_equal(actual.value.interning_failed,
                                               expected.value.interning_failed);
  }
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
  case SMITH_TOKEN_KIND_KEYWORD:
    return smith_assert_keyword_equal(actual.value.keyword,
                                      expected.value.keyword);
  case SMITH_TOKEN_KIND_END_OF_FILE:
    return smith_assert_end_of_file_equal(actual.value.end_of_file,
                                          expected.value.end_of_file);
  case SMITH_TOKEN_KIND_ERROR:
    return smith_assert_error_equal(actual.value.error, expected.value.error);
  }
}

void smith_assert_next_token_result_equal(smith_next_token_result_t actual,
                                          smith_next_token_result_t expected) {
  smith_assert_token_equal(actual.token, expected.token);
  smith_assert_cursor_equal(actual.cursor, expected.cursor);
}

void smith_assert_binary_operator_info_equal(
    smith_binary_operator_info_t actual,
    smith_binary_operator_info_t expected) {
  munit_assert_int(actual.kind, ==, expected.kind);
  munit_assert_int(actual.precedence, ==, expected.precedence);
  munit_assert_int(actual.associativity, ==, expected.associativity);
}

void smith_assert_binary_operator_equal(smith_binary_operator_t actual,
                                        smith_binary_operator_t expected) {
  smith_assert_binary_operator_info_equal(actual.info, expected.info);
  smith_assert_span_equal(actual.op_span, expected.op_span);
  munit_assert_not_null(actual.left);
  smith_assert_expression_equal(*actual.left, *expected.left);
  munit_assert_not_null(actual.right);
  smith_assert_expression_equal(*actual.right, *expected.right);
}

void smith_assert_expression_equal(smith_expression_t actual,
                                   smith_expression_t expected) {
  munit_assert_int(actual.kind, ==, expected.kind);
  switch (actual.kind) {
  case SMITH_EXPRESSION_KIND_SYMBOL:
    return smith_assert_symbol_equal(actual.value.symbol,
                                     expected.value.symbol);
  case SMITH_EXPRESSION_KIND_INT:
    return smith_assert_int_equal(actual.value.int_, expected.value.int_);
  case SMITH_EXPRESSION_KIND_FLOAT:
    return smith_assert_float_equal(actual.value.float_, expected.value.float_);
  case SMITH_EXPRESSION_KIND_BINARY_OPERATOR:
    return smith_assert_binary_operator_equal(actual.value.binary_operator,
                                              expected.value.binary_operator);
  }
}

void smith_assert_parse_result_equal(smith_parse_result_t actual,
                                     smith_parse_result_t expected) {
  smith_assert_expression_equal(actual.expression, expected.expression);
  smith_assert_cursor_equal(actual.cursor, expected.cursor);
}
