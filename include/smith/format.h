#pragma once

#define SMITH_ENABLE_ALLOCATOR_MACROS

#include "smith/allocator.h"

/**
 * Formats a string according to the given format string and additional arguments,
 * similar to sprintf, but allocates memory for the resulting string using the provided allocator.
 * This function is intended for creating formatted strings dynamically with custom memory management.
 *
 * @param allocator The allocator to use for allocating memory for the formatted string.
 * @param format The format string (similar to printf format).
 * @param ... Variable arguments providing additional data to format.
 * @return A pointer to the newly formatted string, or NULL if allocation fails.
 */
char *smith_format_string(smith_allocator_t allocator, const char *format, ...);
