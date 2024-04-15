#define SMITH_ENABLE_ALLOCATOR_MACROS

#include "smith/format.h"
#include <stdarg.h>
#include <stdio.h>

char *smith_format_string(smith_allocator_t allocator, const char *format,
                          ...) {
  va_list args;
  va_start(args, format);
  int size_needed = vsnprintf(NULL, 0, format, args);
  va_end(args);
  char *buffer =
      smith_allocator_allocate_array(allocator, char, size_needed + 1);
  if (buffer == NULL) {
    return NULL;
  }
  va_start(args, format);
  vsnprintf(buffer, size_needed + 1, format, args);
  va_end(args);
  return buffer;
}
