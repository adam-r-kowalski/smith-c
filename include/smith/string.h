#pragma once

#include <stddef.h>

/**
 * Represents a dynamically sized string.
 *
 * @param data Pointer to the character data.
 * @param length The number of characters in the string, not including any null termination.
 */
typedef struct {
  char *data;
  size_t length;
} smith_string_t;
