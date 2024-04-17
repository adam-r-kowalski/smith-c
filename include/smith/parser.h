#pragma once

#include "smith/allocator.h"
#include "smith/tokenizer.h"

typedef struct smith_expression_t smith_expression_t;

typedef enum {
  SMITH_BINARY_OPERATOR_KIND_ADD,
  SMITH_BINARY_OPERATOR_KIND_ADD_ASSIGN,
} smith_binary_operator_kind_t;

typedef enum {
  SMITH_PRECEDENCE_LOWEST,
  SMITH_PRECEDENCE_ADD,
} smith_precedence_t;

typedef enum {
  SMITH_ASSOCIATIVITY_LEFT,
  SMITH_ASSOCIATIVITY_RIGHT,
} smith_associativity_t;

typedef struct {
  smith_binary_operator_kind_t kind;
  smith_span_t span;
  smith_precedence_t precedence;
  smith_associativity_t associativity;
} smith_binary_operator_info_t;

typedef struct {
  smith_binary_operator_info_t info;
  smith_expression_t *left;
  smith_expression_t *right;
} smith_binary_operator_t;

typedef enum {
  SMITH_EXPRESSION_KIND_SYMBOL,
  SMITH_EXPRESSION_KIND_INT,
  SMITH_EXPRESSION_KIND_FLOAT,
  SMITH_EXPRESSION_KIND_BINARY_OPERATOR,
} smith_expression_kind_t;

typedef union {
  smith_symbol_t symbol;
  smith_int_t int_;
  smith_float_t float_;
  smith_binary_operator_t binary_operator;
} smith_expression_value_t;

struct smith_expression_t {
  smith_expression_kind_t kind;
  smith_expression_value_t value;
};

typedef struct {
  smith_expression_t expression;
  smith_cursor_t cursor;
} smith_parse_result_t;

typedef struct {
  smith_allocator_t allocator;
  smith_interner_t interner;
  smith_keywords_t keywords;
} smith_parser_context_t;

smith_parse_result_t smith_parse_expression(smith_parser_context_t context,
                                            smith_cursor_t cursor);

void smith_expression_destroy(smith_allocator_t allocator,
                              smith_expression_t expression);
