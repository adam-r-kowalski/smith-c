#include "smith/tokenizer.h"

smith_next_token_result_t smith_next_token(smith_interner_t intener,
                                           smith_cursor_t cursor) {
  return (smith_next_token_result_t){
      .token = {.kind = SMITH_TOKEN_KIND_END_OF_FILE,
                .value = {.end_of_file = {.span = {.begin = cursor.position,
                                                   .end = cursor.position}}}},
      .cursor = cursor,
  };
}
