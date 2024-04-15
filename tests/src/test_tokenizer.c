#define SMITH_ENABLE_ALLOCATOR_MACROS

#include "smith/assertions.h"
#include "smith/format.h"
#include "smith/hash_interner.h"
#include "smith/random.h"
#include "smith/string.h"
#include "smith/system_allocator.h"
#include "smith/test_suites.h"
#include "smith/tokenizer.h"

static MunitResult test_smith_tokenize_symbol(const MunitParameter params[],
                                              void *user_data_or_fixture) {
  smith_allocator_t allocator = smith_system_allocator_create();
  smith_interner_t interner = smith_hash_interner_create(allocator);
  smith_keywords_t keywords = smith_keywords_create(interner);
  smith_string_t symbol = smith_random_symbol(allocator);
  smith_intern_result_t intern_result = smith_interner_intern(interner, symbol);
  munit_assert(intern_result.success);
  smith_cursor_t cursor = {.source = symbol.data};
  smith_next_token_result_t actual =
      smith_next_token(interner, cursor, keywords);
  smith_position_t end = {.column = symbol.length};
  smith_next_token_result_t expected = {
      .token = {.kind = SMITH_TOKEN_KIND_SYMBOL,
                .value.symbol = {.interned = intern_result.interned,
                                 .span.end = end}},
      .cursor = {.source = "", .position = end}};
  smith_assert_next_token_result_equal(actual, expected);
  smith_interner_destroy(interner);
  smith_allocator_destroy(allocator);
  return MUNIT_OK;
}

static MunitResult test_smith_tokenize_int(const MunitParameter params[],
                                           void *user_data_or_fixture) {
  smith_allocator_t allocator = smith_system_allocator_create();
  smith_interner_t interner = smith_hash_interner_create(allocator);
  smith_keywords_t keywords = smith_keywords_create(interner);
  smith_string_t symbol = smith_random_int(allocator);
  smith_intern_result_t intern_result = smith_interner_intern(interner, symbol);
  munit_assert(intern_result.success);
  smith_cursor_t cursor = {.source = symbol.data};
  smith_next_token_result_t actual =
      smith_next_token(interner, cursor, keywords);
  smith_position_t end = {.column = symbol.length};
  smith_next_token_result_t expected = {
      .token = {.kind = SMITH_TOKEN_KIND_INT,
                .value.int_ = {.interned = intern_result.interned,
                               .span.end = end}},
      .cursor = {.source = "", .position = end}};
  smith_assert_next_token_result_equal(actual, expected);
  smith_interner_destroy(interner);
  smith_allocator_destroy(allocator);
  return MUNIT_OK;
}

static MunitResult test_smith_tokenize_float(const MunitParameter params[],
                                             void *user_data_or_fixture) {
  smith_allocator_t allocator = smith_system_allocator_create();
  smith_interner_t interner = smith_hash_interner_create(allocator);
  smith_keywords_t keywords = smith_keywords_create(interner);
  smith_string_t symbol = smith_random_float(allocator);
  smith_intern_result_t intern_result = smith_interner_intern(interner, symbol);
  munit_assert(intern_result.success);
  smith_cursor_t cursor = {.source = symbol.data};
  smith_next_token_result_t actual =
      smith_next_token(interner, cursor, keywords);
  smith_position_t end = {.column = symbol.length};
  smith_next_token_result_t expected = {
      .token = {.kind = SMITH_TOKEN_KIND_FLOAT,
                .value.float_ = {.interned = intern_result.interned,
                                 .span.end = end}},
      .cursor = {.source = "", .position = end}};
  smith_assert_next_token_result_equal(actual, expected);
  smith_interner_destroy(interner);
  smith_allocator_destroy(allocator);
  return MUNIT_OK;
}

static MunitResult test_smith_tokenize_operator(const MunitParameter params[],
                                                void *user_data_or_fixture) {
  smith_allocator_t allocator = smith_system_allocator_create();
  smith_interner_t interner = smith_hash_interner_create(allocator);
  smith_keywords_t keywords = smith_keywords_create(interner);
  smith_string_t operators[] = {
      {"+", 1},  {"+=", 2}, {"-", 1},  {"-=", 2}, {"*", 1},
      {"*=", 2}, {"/", 1},  {"/=", 2}, {"=", 1},  {"==", 2},
      {"!", 1},  {"!=", 2}, {"<", 1},  {"<=", 2}, {">", 1},
      {">=", 2}, {"&", 1},  {"&&", 2}, {"|", 1},  {"||", 2},
  };
  smith_operator_kind_t kinds[] = {
      SMITH_OPERATOR_KIND_ADD,     SMITH_OPERATOR_KIND_ADD_ASSIGN,
      SMITH_OPERATOR_KIND_SUB,     SMITH_OPERATOR_KIND_SUB_ASSIGN,
      SMITH_OPERATOR_KIND_MUL,     SMITH_OPERATOR_KIND_MUL_ASSIGN,
      SMITH_OPERATOR_KIND_DIV,     SMITH_OPERATOR_KIND_DIV_ASSIGN,
      SMITH_OPERATOR_KIND_ASSIGN,  SMITH_OPERATOR_KIND_EQ,
      SMITH_OPERATOR_KIND_NOT,     SMITH_OPERATOR_KIND_NOT_EQ,
      SMITH_OPERATOR_KIND_LT,      SMITH_OPERATOR_KIND_LE,
      SMITH_OPERATOR_KIND_GT,      SMITH_OPERATOR_KIND_GE,
      SMITH_OPERATOR_KIND_BIT_AND, SMITH_OPERATOR_KIND_AND,
      SMITH_OPERATOR_KIND_BIT_OR,  SMITH_OPERATOR_KIND_OR,
  };
  for (size_t i = 0; i < sizeof(operators) / sizeof(operators[0]); i++) {
    smith_cursor_t cursor = {.source = operators[i].data};
    smith_next_token_result_t actual =
        smith_next_token(interner, cursor, keywords);
    smith_position_t end = {.column = operators[i].length};
    smith_next_token_result_t expected = {
        .token = {.kind = SMITH_TOKEN_KIND_OPERATOR,
                  .value.operator_ = {.kind = kinds[i], .span.end = end}},
        .cursor = {.source = "", .position = end}};
    smith_assert_next_token_result_equal(actual, expected);
  }
  smith_interner_destroy(interner);
  smith_allocator_destroy(allocator);
  return MUNIT_OK;
}

static MunitResult test_smith_tokenize_delimiter(const MunitParameter params[],
                                                 void *user_data_or_fixture) {
  smith_allocator_t allocator = smith_system_allocator_create();
  smith_interner_t interner = smith_hash_interner_create(allocator);
  smith_keywords_t keywords = smith_keywords_create(interner);
  smith_string_t operators[] = {{"(", 1}, {")", 1}, {"{", 1}, {"}", 1},
                                {"[", 1}, {"]", 1}, {",", 1}};
  smith_delimiter_kind_t kinds[] = {
      SMITH_DELIMITER_KIND_OPEN_PAREN,   SMITH_DELIMITER_KIND_CLOSE_PAREN,
      SMITH_DELIMITER_KIND_OPEN_BRACE,   SMITH_DELIMITER_KIND_CLOSE_BRACE,
      SMITH_DELIMITER_KIND_OPEN_BRACKET, SMITH_DELIMITER_KIND_CLOSE_BRACKET,
      SMITH_DELIMITER_KIND_COMMA,
  };
  for (size_t i = 0; i < sizeof(operators) / sizeof(operators[0]); i++) {
    smith_cursor_t cursor = {.source = operators[i].data};
    smith_next_token_result_t actual =
        smith_next_token(interner, cursor, keywords);
    smith_position_t end = {.column = operators[i].length};
    smith_next_token_result_t expected = {
        .token = {.kind = SMITH_TOKEN_KIND_DELIMITER,
                  .value.delimiter = {.kind = kinds[i], .span.end = end}},
        .cursor = {.source = "", .position = end}};
    smith_assert_next_token_result_equal(actual, expected);
  }
  smith_interner_destroy(interner);
  smith_allocator_destroy(allocator);
  return MUNIT_OK;
}

static MunitResult test_smith_tokenize_keyword(const MunitParameter params[],
                                               void *user_data_or_fixture) {
  smith_allocator_t allocator = smith_system_allocator_create();
  smith_interner_t interner = smith_hash_interner_create(allocator);
  smith_keywords_t keywords = smith_keywords_create(interner);
  smith_string_t keyword_strings[] = {{"fn", 2}};
  smith_keyword_kind_t kinds[] = {
      SMITH_KEYWORD_KIND_FN,
  };
  for (size_t i = 0; i < sizeof(keyword_strings) / sizeof(keyword_strings[0]);
       i++) {
    smith_cursor_t cursor = {.source = keyword_strings[i].data};
    smith_next_token_result_t actual =
        smith_next_token(interner, cursor, keywords);
    smith_position_t end = {.column = keyword_strings[i].length};
    smith_next_token_result_t expected = {
        .token = {.kind = SMITH_TOKEN_KIND_KEYWORD,
                  .value.keyword = {.kind = kinds[i], .span.end = end}},
        .cursor = {.source = "", .position = end}};
    smith_assert_next_token_result_equal(actual, expected);
  }
  smith_interner_destroy(interner);
  smith_allocator_destroy(allocator);
  return MUNIT_OK;
}

static MunitResult
test_smith_tokenize_empty_string(const MunitParameter params[],
                                 void *user_data_or_fixture) {
  smith_allocator_t allocator = smith_system_allocator_create();
  smith_interner_t interner = smith_hash_interner_create(allocator);
  smith_keywords_t keywords = smith_keywords_create(interner);
  smith_cursor_t cursor = {.source = ""};
  smith_next_token_result_t actual =
      smith_next_token(interner, cursor, keywords);
  smith_next_token_result_t expected = {
      .token.kind = SMITH_TOKEN_KIND_END_OF_FILE, .cursor.source = ""};
  smith_assert_next_token_result_equal(actual, expected);
  smith_interner_destroy(interner);
  smith_allocator_destroy(allocator);
  return MUNIT_OK;
}

static MunitResult
test_smith_tokenize_invalid_token(const MunitParameter params[],
                                  void *user_data_or_fixture) {
  smith_allocator_t allocator = smith_system_allocator_create();
  smith_interner_t interner = smith_hash_interner_create(allocator);
  smith_keywords_t keywords = smith_keywords_create(interner);
  smith_cursor_t cursor = {.source = ";"};
  smith_next_token_result_t actual =
      smith_next_token(interner, cursor, keywords);
  smith_next_token_result_t expected = {
      .token = {.kind = SMITH_TOKEN_KIND_ERROR,
                .value.error = {.kind = SMITH_ERROR_KIND_UNEXPECTED_CHARACTER,
                                .value.unexpected_character = {.character =
                                                                   ';'}}},
      .cursor = {.source = "", .position.column = 1}};
  smith_assert_next_token_result_equal(actual, expected);
  smith_interner_destroy(interner);
  smith_allocator_destroy(allocator);
  return MUNIT_OK;
}

static MunitResult test_smith_tokenize_function(const MunitParameter params[],
                                                void *user_data_or_fixture) {
  smith_allocator_t allocator = smith_system_allocator_create();
  smith_interner_t interner = smith_hash_interner_create(allocator);
  smith_keywords_t keywords = smith_keywords_create(interner);
  smith_string_t function_name = smith_random_symbol(allocator);
  smith_intern_result_t intern_result =
      smith_interner_intern(interner, function_name);
  munit_assert(intern_result.success);
  smith_interned_t interned_function_name = intern_result.interned;
  char *format_string = "fn %s() {\n"
                        "\n"
                        "}";
  char *source =
      smith_format_string(allocator, format_string, function_name.data);
  smith_cursor_t cursor = {.source = source};
  smith_next_token_result_t actual =
      smith_next_token(interner, cursor, keywords);
  size_t end_column = 2;
  char *remaining_source = source + end_column;
  smith_next_token_result_t expected = {
      .token = {.kind = SMITH_TOKEN_KIND_KEYWORD,
                .value.keyword = {.kind = SMITH_KEYWORD_KIND_FN,
                                  .span.end.column = end_column}},
      .cursor = {.source = remaining_source, .position.column = end_column}};
  smith_assert_next_token_result_equal(actual, expected);

  size_t start_column = end_column + 1;
  end_column += 1 + function_name.length;
  remaining_source = source + end_column;
  actual = smith_next_token(interner, actual.cursor, keywords);
  expected = (smith_next_token_result_t){
      .token = {.kind = SMITH_TOKEN_KIND_SYMBOL,
                .value.symbol = {.interned = interned_function_name,
                                 .span = {.start.column = start_column,
                                          .end.column = end_column}}},
      .cursor = {.source = remaining_source, .position.column = end_column}};
  smith_assert_next_token_result_equal(actual, expected);

  start_column = end_column;
  end_column += 1;
  remaining_source = source + end_column;
  actual = smith_next_token(interner, actual.cursor, keywords);
  expected = (smith_next_token_result_t){
      .token = {.kind = SMITH_TOKEN_KIND_DELIMITER,
                .value.delimiter = {.kind = SMITH_DELIMITER_KIND_OPEN_PAREN,
                                    .span = {.start.column = start_column,
                                             .end.column = end_column}}},
      .cursor = {.source = remaining_source, .position.column = end_column}};
  smith_assert_next_token_result_equal(actual, expected);

  start_column = end_column;
  end_column += 1;
  remaining_source = source + end_column;
  actual = smith_next_token(interner, actual.cursor, keywords);
  expected = (smith_next_token_result_t){
      .token = {.kind = SMITH_TOKEN_KIND_DELIMITER,
                .value.delimiter = {.kind = SMITH_DELIMITER_KIND_CLOSE_PAREN,
                                    .span = {.start.column = start_column,
                                             .end.column = end_column}}},
      .cursor = {.source = remaining_source, .position.column = end_column}};
  smith_assert_next_token_result_equal(actual, expected);

  start_column = end_column + 1;
  end_column += 2;
  remaining_source = source + end_column;
  actual = smith_next_token(interner, actual.cursor, keywords);
  expected = (smith_next_token_result_t){
      .token = {.kind = SMITH_TOKEN_KIND_DELIMITER,
                .value.delimiter = {.kind = SMITH_DELIMITER_KIND_OPEN_BRACE,
                                    .span = {.start.column = start_column,
                                             .end.column = end_column}}},
      .cursor = {.source = remaining_source, .position.column = end_column}};
  smith_assert_next_token_result_equal(actual, expected);

  actual = smith_next_token(interner, actual.cursor, keywords);
  expected = (smith_next_token_result_t){
      .token = {.kind = SMITH_TOKEN_KIND_DELIMITER,
                .value.delimiter = {.kind = SMITH_DELIMITER_KIND_CLOSE_BRACE,
                                    .span = {.start.line = 2,
                                             .end = {.line = 2, .column = 1}}}},
      .cursor = {.source = "", .position = {.line = 2, .column = 1}}};
  smith_assert_next_token_result_equal(actual, expected);

  actual = smith_next_token(interner, actual.cursor, keywords);
  expected = (smith_next_token_result_t){
      .token = {.kind = SMITH_TOKEN_KIND_END_OF_FILE,
                .value.end_of_file.span.start = {.line = 2, .column = 1}},
      .cursor = {.source = "", .position = {.line = 2, .column = 1}}};

  smith_interner_destroy(interner);
  smith_allocator_destroy(allocator);
  return MUNIT_OK;
}

static MunitTest smith_tokenizer_tests[] = {
    {
        .name = "/test_smith_tokenize_symbol",
        .test = test_smith_tokenize_symbol,
    },
    {
        .name = "/test_smith_tokenize_int",
        .test = test_smith_tokenize_int,
    },
    {
        .name = "/test_smith_tokenize_float",
        .test = test_smith_tokenize_float,
    },
    {
        .name = "/test_smith_tokenize_operator",
        .test = test_smith_tokenize_operator,
    },
    {
        .name = "/test_smith_tokenize_delimiter",
        .test = test_smith_tokenize_delimiter,
    },
    {
        .name = "/test_smith_tokenize_keyword",
        .test = test_smith_tokenize_keyword,
    },
    {
        .name = "/test_smith_tokenize_empty_string",
        .test = test_smith_tokenize_empty_string,
    },
    {
        .name = "/test_smith_tokenize_invalid_token",
        .test = test_smith_tokenize_invalid_token,
    },
    {
        .name = "/test_smith_tokenize_function",
        .test = test_smith_tokenize_function,
    },
    {}};

MunitSuite smith_tokenizer_suite = {
    .prefix = "/tokenizer",
    .tests = smith_tokenizer_tests,
    .iterations = 1,
};
