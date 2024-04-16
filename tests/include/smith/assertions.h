#pragma once

#include "smith/parser.h"

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

void smith_assert_keyword_equal(smith_keyword_t actual,
                                smith_keyword_t expected);

void smith_assert_end_of_file_equal(smith_end_of_file_t actual,
                                    smith_end_of_file_t expected);

void smith_assert_unexpected_character_equal(
    smith_unexpected_character_t actual, smith_unexpected_character_t expected);

void smith_assert_interning_failed_equal(smith_interning_failed_t actual,
                                         smith_interning_failed_t expected);

void smith_assert_error_equal(smith_error_t actual, smith_error_t expected);

void smith_assert_token_equal(smith_token_t actual, smith_token_t expected);

void smith_assert_next_token_result_equal(smith_next_token_result_t actual,
                                          smith_next_token_result_t expected);

void smith_assert_expression_equal(smith_expression_t actual,
                                   smith_expression_t expected);

void smith_assert_parse_result_equal(smith_parse_result_t actual,
                                     smith_parse_result_t expected);
