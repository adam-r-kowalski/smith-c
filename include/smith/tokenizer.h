#pragma once

#include "smith/cursor.h"
#include "smith/interner.h"

typedef struct {
  smith_span_t span;
  smith_interned_t interned;
} smith_symbol_t;

typedef struct {
  smith_span_t span;
  smith_interned_t interned;
} smith_float_t;

typedef struct {
  smith_span_t span;
  smith_interned_t interned;
} smith_int_t;

typedef enum {
  SMITH_OPERATOR_KIND_ADD,
  SMITH_OPERATOR_KIND_ADD_ASSIGN,
  SMITH_OPERATOR_KIND_SUB,
  SMITH_OPERATOR_KIND_SUB_ASSIGN,
  SMITH_OPERATOR_KIND_MUL,
  SMITH_OPERATOR_KIND_MUL_ASSIGN,
  SMITH_OPERATOR_KIND_DIV,
  SMITH_OPERATOR_KIND_DIV_ASSIGN,
  SMITH_OPERATOR_KIND_ASSIGN,
  SMITH_OPERATOR_KIND_EQ,
  SMITH_OPERATOR_KIND_NOT,
  SMITH_OPERATOR_KIND_NOT_EQ,
  SMITH_OPERATOR_KIND_LT,
  SMITH_OPERATOR_KIND_LE,
  SMITH_OPERATOR_KIND_GT,
  SMITH_OPERATOR_KIND_GE,
  SMITH_OPERATOR_KIND_BIT_AND,
  SMITH_OPERATOR_KIND_AND,
  SMITH_OPERATOR_KIND_BIT_OR,
  SMITH_OPERATOR_KIND_OR,
} smith_operator_kind_t;

typedef struct {
  smith_span_t span;
  smith_operator_kind_t kind;
} smith_operator_t;

typedef enum {
  SMITH_DELIMITER_KIND_OPEN_PAREN,
  SMITH_DELIMITER_KIND_CLOSE_PAREN,
  SMITH_DELIMITER_KIND_OPEN_BRACE,
  SMITH_DELIMITER_KIND_CLOSE_BRACE,
  SMITH_DELIMITER_KIND_OPEN_BRACKET,
  SMITH_DELIMITER_KIND_CLOSE_BRACKET,
  SMITH_DELIMITER_KIND_COMMA,
} smith_delimiter_kind_t;

typedef struct {
  smith_span_t span;
  smith_delimiter_kind_t kind;
} smith_delimiter_t;

typedef struct {
  smith_span_t span;
} smith_end_of_file_t;

typedef struct {
  smith_span_t span;
  char character;
} smith_unexpected_character_t;

typedef struct {
  smith_span_t span;
  smith_string_t string;
} smith_interning_failed_t;

typedef enum {
  SMITH_ERROR_KIND_UNEXPECTED_CHARACTER,
  SMITH_ERROR_KIND_INTERING_FAILED,
} smith_error_kind_t;

typedef union {
  smith_unexpected_character_t unexpected_character;
  smith_interning_failed_t interning_failed;
} smith_error_value_t;

typedef struct {
  smith_error_kind_t kind;
  smith_error_value_t value;
} smith_error_t;

typedef enum {
  SMITH_TOKEN_KIND_SYMBOL,
  SMITH_TOKEN_KIND_FLOAT,
  SMITH_TOKEN_KIND_INT,
  SMITH_TOKEN_KIND_OPERATOR,
  SMITH_TOKEN_KIND_DELIMITER,
  SMITH_TOKEN_KIND_END_OF_FILE,
  SMITH_TOKEN_KIND_ERROR,
} smith_token_kind_t;

typedef union {
  smith_symbol_t symbol;
  smith_float_t float_;
  smith_int_t int_;
  smith_operator_t operator_;
  smith_delimiter_t delimiter;
  smith_end_of_file_t end_of_file;
  smith_error_t error;
} smith_token_value_t;

typedef struct {
  smith_token_kind_t kind;
  smith_token_value_t value;
} smith_token_t;

typedef struct {
  smith_token_t token;
  smith_cursor_t cursor;
} smith_next_token_result_t;

smith_next_token_result_t smith_next_token(smith_interner_t interner,
                                           smith_cursor_t cursor);
