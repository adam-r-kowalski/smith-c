#pragma once

#include "smith/allocator.h"
#include "smith/tokenizer.h"

/**
 * Forward declaration of the expression structure.
 */
typedef struct smith_expression_t smith_expression_t;

/**
 * Enumeration of binary operator kinds supported by the parser.
 */
typedef enum {
  SMITH_BINARY_OPERATOR_KIND_ADD,
  SMITH_BINARY_OPERATOR_KIND_ADD_ASSIGN,
  SMITH_BINARY_OPERATOR_KIND_SUB,
  SMITH_BINARY_OPERATOR_KIND_SUB_ASSIGN,
  SMITH_BINARY_OPERATOR_KIND_MUL,
  SMITH_BINARY_OPERATOR_KIND_MUL_ASSIGN,
  SMITH_BINARY_OPERATOR_KIND_DIV,
  SMITH_BINARY_OPERATOR_KIND_DIV_ASSIGN,
  SMITH_BINARY_OPERATOR_KIND_ASSIGN,
  SMITH_BINARY_OPERATOR_KIND_EQ,
  SMITH_BINARY_OPERATOR_KIND_NOT,
  SMITH_BINARY_OPERATOR_KIND_NOT_EQ,
  SMITH_BINARY_OPERATOR_KIND_LT,
  SMITH_BINARY_OPERATOR_KIND_LE,
  SMITH_BINARY_OPERATOR_KIND_GT,
  SMITH_BINARY_OPERATOR_KIND_GE,
  SMITH_BINARY_OPERATOR_KIND_BIT_AND,
  SMITH_BINARY_OPERATOR_KIND_AND,
  SMITH_BINARY_OPERATOR_KIND_BIT_OR,
  SMITH_BINARY_OPERATOR_KIND_OR,
} smith_binary_operator_kind_t;

/**
 * Enumeration of operator precedence levels.
 */
typedef enum {
  SMITH_PRECEDENCE_LOWEST,
  SMITH_PRECEDENCE_ASSIGN,
  SMITH_PRECEDENCE_ADD,
  SMITH_PRECEDENCE_MUL,
  SMITH_PRECEDENCE_EQ,
} smith_precedence_t;

/**
 * Enumeration of operator associativity types.
 */
typedef enum {
  SMITH_ASSOCIATIVITY_LEFT,  // Left-to-right associativity
  SMITH_ASSOCIATIVITY_RIGHT  // Right-to-left associativity
} smith_associativity_t;

/**
 * Represents information about a binary operator including its kind,
 * precedence, and associativity.
 */
typedef struct {
  smith_binary_operator_kind_t kind;
  smith_precedence_t precedence;
  smith_associativity_t associativity;
} smith_binary_operator_info_t;

/**
 * Represents a binary operator expression in the AST.
 *
 * @param info Metadata about the operator (type, precedence, associativity).
 * @param op_span The span of the operator in the source text.
 * @param left Pointer to the left operand expression.
 * @param right Pointer to the right operand expression.
 */
typedef struct {
  smith_binary_operator_info_t info;
  smith_span_t op_span;
  smith_expression_t *left;
  smith_expression_t *right;
} smith_binary_operator_t;

/**
 * Enumeration of expression kinds.
 */
typedef enum {
  SMITH_EXPRESSION_KIND_SYMBOL,
  SMITH_EXPRESSION_KIND_INT,
  SMITH_EXPRESSION_KIND_FLOAT,
  SMITH_EXPRESSION_KIND_BINARY_OPERATOR,
} smith_expression_kind_t;

/**
 * Union of all possible expression values.
 */
typedef union {
  smith_symbol_t symbol;
  smith_int_t int_;
  smith_float_t float_;
  smith_binary_operator_t binary_operator;
} smith_expression_value_t;

/**
 * Represents an expression in the AST.
 *
 * @param kind The type of expression.
 * @param value The value of the expression, which varies based on the expression kind.
 */
struct smith_expression_t {
  smith_expression_kind_t kind;
  smith_expression_value_t value;
};

/**
 * Represents the result of parsing an expression.
 *
 * @param expression The parsed expression.
 * @param cursor Position in the source text after parsing the expression.
 */
typedef struct {
  smith_expression_t expression;
  smith_cursor_t cursor;
} smith_parse_result_t;

/**
 * Represents the context for the parser, including memory allocation and keyword handling.
 *
 * @param allocator Memory allocator for dynamic allocations within the parser.
 * @param interner Interner for managing unique strings and identifiers.
 * @param keywords Set of keywords recognized by the parser.
 */
typedef struct {
  smith_allocator_t allocator;
  smith_interner_t interner;
  smith_keywords_t keywords;
} smith_parser_context_t;

/**
 * Parses an expression from the source text starting at the given cursor position.
 *
 * @param context The parser context containing all necessary resources.
 * @param cursor The current position in the source text.
 * @return The result of the parsing operation, including the parsed expression and the new cursor position.
 */
smith_parse_result_t smith_parse_expression(smith_parser_context_t context,
                                            smith_cursor_t cursor);

/**
 * Destroys an expression, releasing any resources it holds.
 *
 * @param allocator The allocator used to manage memory for expressions.
 * @param expression The expression to destroy.
 */
void smith_expression_destroy(smith_allocator_t allocator,
                              smith_expression_t expression);

/**
 * Externally defined array of binary operator information, mapping operator kinds
 * to their respective precedence and associativity.
 */
extern smith_binary_operator_info_t smith_binary_operator_mapping[];
