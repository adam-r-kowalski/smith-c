#define SMITH_ENABLE_ALLOCATOR_MACROS

#include "smith/tokenizer.h"
#include "smith/interner.h"
#include <assert.h>

typedef struct {
  smith_string_t string;
  smith_cursor_t cursor;
  void *state;
} take_while_stateful_result_t;

static take_while_stateful_result_t
take_while_stateful(smith_cursor_t cursor, bool (*predicate)(char, void *),
                    void *state) {
  size_t length = 0;
  while (cursor.source[length] != '\0' &&
         predicate(cursor.source[length], state)) {
    length++;
  }
  return (take_while_stateful_result_t){
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
  take_while_stateful_result_t result =
      take_while_stateful(cursor, stateless_predicate, predicate);
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
                                                 smith_cursor_t cursor,
                                                 smith_keywords_t keywords) {
  take_while_result_t take_while_result = take_while(cursor, is_symbol_char);
  smith_intern_result_t intern_result =
      smith_interner_intern(intener, take_while_result.string);
  if (intern_result.success) {
    smith_span_t span = {.start = cursor.position,
                         .end = take_while_result.cursor.position};
    if (intern_result.interned == keywords.fn) {
      return (smith_next_token_result_t){
          .token = {.kind = SMITH_TOKEN_KIND_KEYWORD,
                    .value.keyword = {.kind = SMITH_KEYWORD_KIND_FN,
                                      .span = span}},
          .cursor = take_while_result.cursor,
      };
    }
    return (smith_next_token_result_t){
        .token = {.kind = SMITH_TOKEN_KIND_SYMBOL,
                  .value.symbol = {.interned = intern_result.interned,
                                   .span = span}},
        .cursor = take_while_result.cursor,
    };
  }
  return (smith_next_token_result_t){
      .token = {.kind = SMITH_TOKEN_KIND_ERROR,
                .value.error =
                    {.kind = SMITH_ERROR_KIND_INTERING_FAILED,
                     .value = {.interning_failed =
                                   {.string = take_while_result.string,
                                    .span = {.start = cursor.position,
                                             .end = take_while_result.cursor
                                                        .position}}}}},
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
  take_while_stateful_result_t take_while_result =
      take_while_stateful(cursor, is_numeric, &decimals);
  smith_intern_result_t intern_result =
      smith_interner_intern(intener, take_while_result.string);
  if (!intern_result.success) {
    return (smith_next_token_result_t){
        .token =
            {.kind = SMITH_TOKEN_KIND_ERROR,
             .value = {.error = {.kind = SMITH_ERROR_KIND_INTERING_FAILED,
                                 .value = {.interning_failed =
            {.string = take_while_result.string, .span = {.start = cursor.position,
                                                         .end = take_while_result.cursor.position}}}}}},
        .cursor = take_while_result.cursor,
    };
  }
  smith_span_t span = {.start = cursor.position,
                       .end = take_while_result.cursor.position};
  if (decimals >= 1) {
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

static smith_next_token_result_t
tokenize_operator(smith_cursor_t cursor, smith_operator_kind_t kind, char next,
                  smith_operator_kind_t next_kind) {
  size_t length = 1;
  if (cursor.source[length] == next) {
    length = 2;
    kind = next_kind;
  }
  smith_position_t end = {.line = cursor.position.line,
                          .column = cursor.position.column + length};
  return (smith_next_token_result_t){
      .token = {.kind = SMITH_TOKEN_KIND_OPERATOR,
                .value.operator_ = {.kind = kind,
                                    .span = {.start = cursor.position,
                                             .end = end}}},
      .cursor = {.source = cursor.source + length, .position = end},
  };
}

static smith_next_token_result_t
tokenize_delimiter(smith_cursor_t cursor, smith_delimiter_kind_t kind) {
  smith_position_t end = {.line = cursor.position.line,
                          .column = cursor.position.column + 1};
  return (smith_next_token_result_t){
      .token = {.kind = SMITH_TOKEN_KIND_DELIMITER,
                .value.delimiter = {.kind = kind,
                                    .span = {.start = cursor.position,
                                             .end = end}}},
      .cursor = {.source = cursor.source + 1, .position = end},
  };
}

static smith_next_token_result_t
tokenize_unexpected_character(smith_cursor_t cursor, char c) {
  return (smith_next_token_result_t){
      .token = {.kind = SMITH_TOKEN_KIND_ERROR,
                .value.error = {.kind = SMITH_ERROR_KIND_UNEXPECTED_CHARACTER,
                                .value.unexpected_character =
                                    {.character = c,
                                     .span = {.start = cursor.position,
                                              .end = cursor.position}}}},
      .cursor = {.source = cursor.source + 1,
                 .position = {.line = cursor.position.line,
                              .column = cursor.position.column + 1}},
  };
}

smith_keywords_t smith_keywords_create(smith_interner_t interner) {
  smith_keywords_t keywords;
  smith_intern_result_t intern_result = smith_interner_intern(
      interner, (smith_string_t){.data = "fn", .length = 2});
  assert(intern_result.success);
  keywords.fn = intern_result.interned;
  return keywords;
}

smith_cursor_t trim_whitespace(smith_cursor_t cursor) {
  while (true) {
    switch (*cursor.source) {
    case ' ':
    case '\t':
      cursor.position.column++;
      break;
    case '\n':
      cursor.position.line++;
      cursor.position.column = 0;
      break;
    default:
      return cursor;
    }
    cursor.source++;
  }
}

#define tokenize_operator(kind, next_char, next_kind)                          \
  tokenize_operator(cursor, SMITH_OPERATOR_KIND_##kind, next_char,             \
                    SMITH_OPERATOR_KIND_##next_kind)

smith_next_token_result_t smith_next_token(smith_interner_t intener,
                                           smith_cursor_t cursor,
                                           smith_keywords_t keywords) {
  cursor = trim_whitespace(cursor);
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
    return tokenize_symbol(intener, cursor, keywords);
  case '0' ... '9':
    return tokenize_number(intener, cursor, 0);
  case '.':
    return tokenize_number(intener, cursor, 1);
  case '+':
    return tokenize_operator(ADD, '=', ADD_ASSIGN);
  case '-':
    return tokenize_operator(SUB, '=', SUB_ASSIGN);
  case '*':
    return tokenize_operator(MUL, '=', MUL_ASSIGN);
  case '/':
    return tokenize_operator(DIV, '=', DIV_ASSIGN);
  case '=':
    return tokenize_operator(ASSIGN, '=', EQ);
  case '!':
    return tokenize_operator(NOT, '=', NOT_EQ);
  case '<':
    return tokenize_operator(LT, '=', LE);
  case '>':
    return tokenize_operator(GT, '=', GE);
  case '&':
    return tokenize_operator(BIT_AND, '&', AND);
  case '|':
    return tokenize_operator(BIT_OR, '|', OR);
  case '(':
    return tokenize_delimiter(cursor, SMITH_DELIMITER_KIND_OPEN_PAREN);
  case ')':
    return tokenize_delimiter(cursor, SMITH_DELIMITER_KIND_CLOSE_PAREN);
  case '{':
    return tokenize_delimiter(cursor, SMITH_DELIMITER_KIND_OPEN_BRACE);
  case '}':
    return tokenize_delimiter(cursor, SMITH_DELIMITER_KIND_CLOSE_BRACE);
  case '[':
    return tokenize_delimiter(cursor, SMITH_DELIMITER_KIND_OPEN_BRACKET);
  case ']':
    return tokenize_delimiter(cursor, SMITH_DELIMITER_KIND_CLOSE_BRACKET);
  case ',':
    return tokenize_delimiter(cursor, SMITH_DELIMITER_KIND_COMMA);
  default:
    return tokenize_unexpected_character(cursor, cursor.source[0]);
  }
}

#undef tokenize_operator
