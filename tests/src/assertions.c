#include "smith/assertions.h"
#include <munit.h>

void smith_assert_position_equal(smith_position_t actual,
                                 smith_position_t expected) {}

void smith_assert_span_equal(smith_span_t actual, smith_span_t expected) {}

void smith_assert_cursor_equal(smith_cursor_t actual, smith_cursor_t expected) {
  smith_assert_position_equal(actual.position, expected.position);
  munit_assert_string_equal(actual.source, expected.source);
}

void smith_assert_token_value_equal(smith_token_value_t actual,
                                    smith_token_value_t expected) {}

void smith_assert_token_equal(smith_token_t actual, smith_token_t expected) {}

void smith_assert_next_token_result_equal(smith_next_token_result_t actual,
                                          smith_next_token_result_t expected) {
  smith_assert_token_equal(actual.token, expected.token);
  smith_assert_cursor_equal(actual.cursor, expected.cursor);
}
