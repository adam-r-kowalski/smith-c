#pragma once

#include "smith/cursor.h"
#include "smith/interner.h"

/**
 * Represents a symbol within the source text.
 * 
 * @param span The location of the symbol in the source text.
 * @param interned The interned identifier for the symbol.
 */
typedef struct {
  smith_span_t span;
  smith_interned_t interned;
} smith_symbol_t;

/**
 * Represents a floating-point number within the source text.
 *
 * @param span The location of the floating-point number in the source text.
 * @param interned The interned identifier for the floating-point value.
 */
typedef struct {
  smith_span_t span;
  smith_interned_t interned;
} smith_float_t;

/**
 * Represents an integer within the source text.
 *
 * @param span The location of the integer in the source text.
 * @param interned The interned identifier for the integer value.
 */
typedef struct {
  smith_span_t span;
  smith_interned_t interned;
} smith_int_t;

/**
 * Enumeration of all operator kinds recognized by the tokenizer.
 */
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

/**
 * Represents an operator in the source text.
 *
 * @param span The location of the operator in the source text.
 * @param kind The specific type of operator.
 */
typedef struct {
  smith_span_t span;
  smith_operator_kind_t kind;
} smith_operator_t;

/**
 * Enumeration of all delimiter kinds recognized by the tokenizer.
 */
typedef enum {
  SMITH_DELIMITER_KIND_OPEN_PAREN,
  SMITH_DELIMITER_KIND_CLOSE_PAREN,
  SMITH_DELIMITER_KIND_OPEN_BRACE,
  SMITH_DELIMITER_KIND_CLOSE_BRACE,
  SMITH_DELIMITER_KIND_OPEN_BRACKET,
  SMITH_DELIMITER_KIND_CLOSE_BRACKET,
  SMITH_DELIMITER_KIND_COMMA,
} smith_delimiter_kind_t;

/**
 * Represents a delimiter in the source text.
 *
 * @param span The location of the delimiter in the source text.
 * @param kind The specific type of delimiter.
 */
typedef struct {
  smith_span_t span;
  smith_delimiter_kind_t kind;
} smith_delimiter_t;

/**
 * Enumeration of all keyword kinds recognized by the tokenizer.
 */
typedef enum {
  SMITH_KEYWORD_KIND_FN,  // Represents the "function" keyword in many languages.
} smith_keyword_kind_t;

/**
 * Represents a keyword in the source text.
 *
 * @param span The location of the keyword in the source text.
 * @param kind The specific type of keyword.
 */
typedef struct {
  smith_span_t span;
  smith_keyword_kind_t kind;
} smith_keyword_t;

/**
 * Represents the end of file marker.
 *
 * @param span The location where the end of file was detected.
 */
typedef struct {
  smith_span_t span;
} smith_end_of_file_t;

/**
 * Represents an unexpected character error in the source text.
 *
 * @param span The location of the unexpected character.
 * @param character The unexpected character that was encountered.
 */
typedef struct {
  smith_span_t span;
  char character;
} smith_unexpected_character_t;

/**
 * Represents an error when interning a string fails.
 *
 * @param span The location of the string that failed to be interned.
 * @param string The string that failed interning.
 */
typedef struct {
  smith_span_t span;
  smith_string_t string;
} smith_interning_failed_t;

/**
 * Enumeration of all error kinds that can be encountered by the tokenizer.
 */
typedef enum {
  SMITH_ERROR_KIND_UNEXPECTED_CHARACTER,
  SMITH_ERROR_KIND_INTERNING_FAILED,
} smith_error_kind_t;

/**
 * Union of all possible error values.
 */
typedef union {
  smith_unexpected_character_t unexpected_character;
  smith_interning_failed_t interning_failed;
} smith_error_value_t;

/**
 * Represents an error encountered during tokenization.
 *
 * @param kind The type of error.
 * @param value The specific error information.
 */
typedef struct {
  smith_error_kind_t kind;
  smith_error_value_t value;
} smith_error_t;

/**
 * Enumeration of all token kinds produced by the tokenizer.
 */
typedef enum {
  SMITH_TOKEN_KIND_SYMBOL,
  SMITH_TOKEN_KIND_FLOAT,
  SMITH_TOKEN_KIND_INT,
  SMITH_TOKEN_KIND_OPERATOR,
  SMITH_TOKEN_KIND_DELIMITER,
  SMITH_TOKEN_KIND_KEYWORD,
  SMITH_TOKEN_KIND_END_OF_FILE,
  SMITH_TOKEN_KIND_ERROR,
} smith_token_kind_t;

/**
 * Union of all possible token values.
 */
typedef union {
  smith_symbol_t symbol;
  smith_float_t float_;
  smith_int_t int_;
  smith_operator_t operator_;
  smith_delimiter_t delimiter;
  smith_keyword_t keyword;
  smith_end_of_file_t end_of_file;
  smith_error_t error;
} smith_token_value_t;

/**
 * Represents a token produced by the tokenizer.
 *
 * @param kind The type of token.
 * @param value The value of the token.
 */
typedef struct {
  smith_token_kind_t kind;
  smith_token_value_t value;
} smith_token_t;

/**
 * Represents a collection of keyword identifiers.
 *
 * @param fn The interned identifier for the "fn" keyword.
 */
typedef struct {
  smith_interned_t fn;
} smith_keywords_t;

/**
 * Represents the result of fetching the next token from the source text.
 *
 * @param token The next token.
 * @param cursor The updated position in the source text after fetching the token.
 */
typedef struct {
  smith_token_t token;
  smith_cursor_t cursor;
} smith_next_token_result_t;

/**
 * Represents the result of creating a set of keywords for use in tokenization.
 * This structure provides feedback on the success of the operation along with the created keywords.
 *
 * @param keywords The set of keywords successfully created and interned.
 * @param success Boolean flag indicating whether the keyword creation was successful.
 */
typedef struct {
  smith_keywords_t keywords;  // Contains the interned identifiers for each recognized keyword.
  bool success;               // True if all keywords were successfully created, false otherwise.
} smith_keywords_create_result_t;

/**
 * Creates a collection of keywords from the given interner.
 *
 * @param interner The interner to use for creating keyword identifiers.
 * @return The result containing the created keywords or an indication of failure.
 */
smith_keywords_create_result_t smith_keywords_create(smith_interner_t interner);

/**
 * Fetches the next token from the source text using the given interner and keywords.
 *
 * @param interner The interner to use for string interning.
 * @param cursor The current position in the source text.
 * @param keywords The set of keywords to recognize.
 * @return The result of the tokenization attempt.
 */
smith_next_token_result_t smith_next_token(smith_interner_t interner,
                                           smith_cursor_t cursor,
                                           smith_keywords_t keywords);
