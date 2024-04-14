#pragma once

#include "smith/tokenizer.h"

void smith_assert_position_equal(smith_position_t actual,
                                 smith_position_t expected);

void smith_assert_span_equal(smith_span_t actual, smith_span_t expected);

void smith_assert_cursor_equal(smith_cursor_t actual, smith_cursor_t expected);

void smith_assert_symbol_equal(smith_symbol_t actual, smith_symbol_t expected);

void smith_assert_int_equal(smith_int_t actual, smith_int_t expected);

void smith_assert_float_equal(smith_float_t actual, smith_float_t expected);

void smith_assert_operator_equal(smith_operator_t actual,
                                 smith_operator_t expected);

void smith_assert_delimiter_equal(smith_delimiter_t actual,
                                  smith_delimiter_t expected);

void smith_assert_token_equal(smith_token_t actual, smith_token_t expected);

void smith_assert_next_token_result_equal(smith_next_token_result_t actual,
                                          smith_next_token_result_t expected);
