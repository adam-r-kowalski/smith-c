#include "smith/parser.h"
#include <assert.h>

smith_parse_result_t smith_parse_expression(smith_allocator_t allocator,
                                            smith_interner_t interner,
                                            smith_cursor_t cursor,
                                            smith_keywords_t keywords) {
  smith_next_token_result_t next_token_result =
      smith_next_token(interner, cursor, keywords);
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
