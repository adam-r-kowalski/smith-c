#include "smith/tokenizer.h"
#include <assert.h>

typedef struct {
  smith_string_t string;
  smith_cursor_t cursor;
  void *state;
} take_while_statefull_result_t;

static take_while_statefull_result_t
take_while_statefull(smith_cursor_t cursor, bool (*predicate)(char, void *),
                     void *state) {
  size_t length = 0;
  while (cursor.source[length] != '\0' &&
         predicate(cursor.source[length], state)) {
    length++;
  }
  return (take_while_statefull_result_t){
      .string = {.data = cursor.source, .length = length},
      .cursor = {.source = cursor.source + length,
                 .position = {.line = cursor.position.line,
                              .column = cursor.position.column + length}},
      .state = state,
  };
}

typedef struct {
  smith_string_t string;
  smith_cursor_t cursor;
} take_while_result_t;

static bool stateless_predicate(char c, void *state) {
  bool (*predicate)(char) = state;
  return predicate(c);
}

static take_while_result_t take_while(smith_cursor_t cursor,
                                      bool (*predicate)(char)) {
  take_while_statefull_result_t result =
      take_while_statefull(cursor, stateless_predicate, predicate);
  return (take_while_result_t){
      .string = result.string,
      .cursor = result.cursor,
  };
}

static bool is_symbol_char(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
         (c >= '0' && c <= '9') || c == '_';
}

static smith_next_token_result_t tokenize_symbol(smith_interner_t intener,
                                                 smith_cursor_t cursor) {
  take_while_result_t take_while_result = take_while(cursor, is_symbol_char);
  smith_intern_result_t intern_result =
      smith_interner_intern(intener, take_while_result.string);
  assert(intern_result.success);
  return (smith_next_token_result_t){
      .token = {.kind = SMITH_TOKEN_KIND_SYMBOL,
                .value = {.symbol = {.interned = intern_result.interned,
                                     .span = {.start = cursor.position,
                                              .end = take_while_result.cursor
                                                         .position}}}},
      .cursor = take_while_result.cursor,
  };
}

static bool is_numeric(char c, void *state) {
  uint8_t *decimals = state;
  switch (c) {
  case '0' ... '9':
    return true;
  case '.':
    (*decimals)++;
    return true;
  default:
    return false;
  }
}

static smith_next_token_result_t tokenize_number(smith_interner_t intener,
                                                 smith_cursor_t cursor,
                                                 uint8_t decimals) {
  take_while_statefull_result_t take_while_result =
      take_while_statefull(cursor, is_numeric, &decimals);
  smith_intern_result_t intern_result =
      smith_interner_intern(intener, take_while_result.string);
  assert(intern_result.success);
  smith_span_t span = {.start = cursor.position,
                       .end = take_while_result.cursor.position};
  if (decimals > 1) {
    return (smith_next_token_result_t){
        .token = {.kind = SMITH_TOKEN_KIND_FLOAT,
                  .value = {.float_ = {.interned = intern_result.interned,
                                       .span = span}}},
        .cursor = take_while_result.cursor,
    };
  }
  return (smith_next_token_result_t){
      .token = {.kind = SMITH_TOKEN_KIND_INT,
                .value = {.int_ = {.interned = intern_result.interned,
                                   .span = span}}},
      .cursor = take_while_result.cursor,
  };
}

smith_next_token_result_t smith_next_token(smith_interner_t intener,
                                           smith_cursor_t cursor) {
  if (cursor.source[0] == '\0') {
    return (smith_next_token_result_t){
        .token = {.kind = SMITH_TOKEN_KIND_END_OF_FILE,
                  .value = {.end_of_file = {.span = {.start = cursor.position,
                                                     .end = cursor.position}}}},
        .cursor = cursor,
    };
  }
  switch (cursor.source[0]) {
  case 'a' ... 'z':
  case 'A' ... 'Z':
  case '_':
    return tokenize_symbol(intener, cursor);
  case '0' ... '9':
    return tokenize_number(intener, cursor, 0);
  case '.':
    return tokenize_number(intener, cursor, 1);
  default:
    assert(false);
  }
}
