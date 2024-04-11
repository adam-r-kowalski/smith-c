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
  SMITH_OPERATOR_KIND_PLUS,
} smith_operator_kind_t;

typedef struct {
  smith_span_t span;
  smith_operator_kind_t kind;
} smith_operator_t;

typedef enum {
  SMITH_DELIMITER_KIND_OPEN_PAREN,
} smith_delimiter_kind_t;

typedef struct {
  smith_span_t span;
  smith_delimiter_kind_t kind;
} smith_delimiter_t;

typedef struct {
  smith_span_t span;
} smith_end_of_file_t;

typedef enum {
  SMITH_TOKEN_KIND_SYMBOL,
  SMITH_TOKEN_KIND_FLOAT,
  SMITH_TOKEN_KIND_INT,
  SMITH_TOKEN_KIND_OPERATOR,
  SMITH_TOKEN_KIND_DELIMITER,
  SMITH_TOKEN_KIND_END_OF_FILE,
} smith_token_kind_t;

typedef union {
  smith_symbol_t symbol;
  smith_float_t float_;
  smith_int_t int_;
  smith_operator_t operator;
  smith_delimiter_t delimiter;
  smith_end_of_file_t end_of_file;
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
