#pragma once

#include <stdint.h>

/**
 * Represents a position in a source file or text stream.
 *
 * @param line The line number, starting from 1.
 * @param column The column number, starting from 1, which represents a character position in a line.
 */
typedef struct {
  uint32_t line;   // Line number in the source file.
  uint32_t column; // Column number in the source file.
} smith_position_t;

/**
 * Represents a span in the source text, defined by a starting and ending position.
 * This can be used to identify the location of tokens, errors, or any substring within the source.
 *
 * @param start The starting position of the span.
 * @param end The ending position of the span.
 */
typedef struct {
  smith_position_t start; // Start position of the span.
  smith_position_t end;   // End position of the span.
} smith_span_t;

/**
 * Represents a cursor for text parsing that keeps track of the current position in the source text
 * and the associated character data.
 *
 * @param source Pointer to the current character in the source text.
 * @param position The current position in the source text where the cursor is located.
 */
typedef struct {
  char *source;          // Current parsing location in the source text.
  smith_position_t position; // Current line and column position in the source text.
} smith_cursor_t;
