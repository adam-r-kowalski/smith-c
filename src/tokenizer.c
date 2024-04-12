#include "smith/tokenizer.h"
#include <assert.h>

typedef struct {
  smith_string_t string;
  smith_cursor_t cursor;
} take_while_result_t;

static take_while_result_t take_while(smith_cursor_t cursor,
                                      bool (*predicate)(char)) {
  size_t length = 0;
  while (cursor.source[length] != '\0' && predicate(cursor.source[length])) {
    length++;
  }
  return (take_while_result_t){
      .string = (smith_string_t){.data = cursor.source, .length = length},
      .cursor = (smith_cursor_t){.source = cursor.source + length,
                                 .position = {.line = cursor.position.line,
                                              .column = cursor.position.column +
                                                        length}},
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
                                     .span = {.begin = cursor.position,
                                              .end = take_while_result.cursor
                                                         .position}}}},
      .cursor = take_while_result.cursor,
  };
}

smith_next_token_result_t smith_next_token(smith_interner_t intener,
                                           smith_cursor_t cursor) {
  if (cursor.source[0] == '\0') {
    return (smith_next_token_result_t){
        .token = {.kind = SMITH_TOKEN_KIND_END_OF_FILE,
                  .value = {.end_of_file = {.span = {.begin = cursor.position,
                                                     .end = cursor.position}}}},
        .cursor = cursor,
    };
  }
  switch (cursor.source[0]) {
  case 'a' ... 'z':
  case 'A' ... 'Z':
  case '_':
    return tokenize_symbol(intener, cursor);
  }
}
