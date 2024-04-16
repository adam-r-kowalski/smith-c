#pragma once

#include "smith/allocator.h"
#include "smith/tokenizer.h"

typedef enum {
  SMITH_EXPRESSION_KIND_SYMBOL,
} smith_expression_kind_t;

typedef union {
  smith_symbol_t symbol;
} smith_expression_value_t;

typedef struct {
  smith_expression_kind_t kind;
  smith_expression_value_t value;
} smith_expression_t;

typedef struct {
  smith_expression_t expression;
  smith_cursor_t cursor;
} smith_parse_result_t;

smith_parse_result_t smith_parse_expression(smith_allocator_t allocator,
                                            smith_interner_t interner,
                                            smith_cursor_t cursor,
                                            smith_keywords_t keywords);
