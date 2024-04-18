#define SMITH_ENABLE_ALLOCATOR_MACROS

#include "smith/parser.h"
#include <assert.h>

static smith_next_token_result_t next_token(smith_parser_context_t context,
                                            smith_cursor_t cursor) {
  return smith_next_token(context.interner, cursor, context.keywords);
}

static smith_parse_result_t smith_parse_prefix(smith_parser_context_t context,
                                               smith_cursor_t cursor) {
  smith_next_token_result_t next_token_result = next_token(context, cursor);
  switch (next_token_result.token.kind) {
  case SMITH_TOKEN_KIND_SYMBOL:
    return (smith_parse_result_t){
        .expression = {.kind = SMITH_EXPRESSION_KIND_SYMBOL,
                       .value.symbol = next_token_result.token.value.symbol},
        .cursor = next_token_result.cursor};
  case SMITH_TOKEN_KIND_INT:
    return (smith_parse_result_t){
        .expression = {.kind = SMITH_EXPRESSION_KIND_INT,
                       .value.int_ = next_token_result.token.value.int_},
        .cursor = next_token_result.cursor};
  case SMITH_TOKEN_KIND_FLOAT:
    return (smith_parse_result_t){
        .expression = {.kind = SMITH_EXPRESSION_KIND_FLOAT,
                       .value.float_ = next_token_result.token.value.float_},
        .cursor = next_token_result.cursor};
  default:
    assert(false);
  }
}

typedef enum {
  INFIX_PARSER_KIND_BINARY_OPERATOR,
  INFIX_PARSER_KIND_NONE,
} infix_parser_kind_t;

typedef union {
  smith_binary_operator_info_t binary_operator_info;
  void *none;
} infix_parser_value_t;

typedef struct {
  infix_parser_kind_t kind;
  infix_parser_value_t value;
} infix_parser_t;

static infix_parser_t infix_parser_for(smith_token_t token) {
  switch (token.kind) {
  case SMITH_TOKEN_KIND_OPERATOR: {
    smith_operator_kind_t kind = token.value.operator_.kind;
    return (infix_parser_t){
        .kind = INFIX_PARSER_KIND_BINARY_OPERATOR,
        .value = {
            .binary_operator_info = {
                .kind = smith_binary_operator_kinds[kind],
                .span = token.value.operator_.span,
                .precedence = smith_binary_operator_precedences[kind],
                .associativity = smith_binary_operator_associativities[kind]}}};
  }
  default:
    return (infix_parser_t){.kind = INFIX_PARSER_KIND_NONE,
                            .value = {.none = nullptr}};
  }
}

static smith_parse_result_t smith_parse_binary_operator(
    smith_parser_context_t context, smith_cursor_t cursor,
    smith_binary_operator_info_t info, smith_expression_t left) {
  smith_expression_t *left_ =
      smith_allocator_allocate(context.allocator, smith_expression_t);
  // TODO: Handle allocation failure.
  assert(left_ != nullptr);
  *left_ = left;
  smith_parse_result_t right_parse_result =
      smith_parse_expression(context, cursor);
  smith_expression_t *right =
      smith_allocator_allocate(context.allocator, smith_expression_t);
  // TODO: Handle allocation failure.
  assert(right != nullptr);
  *right = right_parse_result.expression;
  return (smith_parse_result_t){
      .expression = {.kind = SMITH_EXPRESSION_KIND_BINARY_OPERATOR,
                     .value.binary_operator = {.info = info,
                                               .left = left_,
                                               .right = right}},
      .cursor = right_parse_result.cursor};
}

smith_parse_result_t smith_parse_expression(smith_parser_context_t context,
                                            smith_cursor_t cursor) {
  smith_parse_result_t prefix_parse_result =
      smith_parse_prefix(context, cursor);
  smith_next_token_result_t next_token_result =
      next_token(context, prefix_parse_result.cursor);
  infix_parser_t infix_parser = infix_parser_for(next_token_result.token);
  switch (infix_parser.kind) {
  case INFIX_PARSER_KIND_BINARY_OPERATOR: {
    return smith_parse_binary_operator(context, next_token_result.cursor,
                                       infix_parser.value.binary_operator_info,
                                       prefix_parse_result.expression);
  }
  case INFIX_PARSER_KIND_NONE:
    return prefix_parse_result;
  }
}

void smith_expression_destroy(smith_allocator_t allocator,
                              smith_expression_t expression) {
  switch (expression.kind) {
  case SMITH_EXPRESSION_KIND_BINARY_OPERATOR: {
    smith_expression_t *left = expression.value.binary_operator.left;
    smith_expression_t *right = expression.value.binary_operator.right;
    smith_expression_destroy(allocator, *left);
    smith_expression_destroy(allocator, *right);
    smith_allocator_deallocate(allocator, left);
    smith_allocator_deallocate(allocator, right);
    break;
  }
  default:
    break;
  }
}
