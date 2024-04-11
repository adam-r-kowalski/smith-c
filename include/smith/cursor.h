#pragma once

#include <stdint.h>

typedef struct {
  uint32_t line;
  uint32_t column;
} smith_position_t;

typedef struct {
  smith_position_t begin;
  smith_position_t end;
} smith_span_t;

typedef struct {
  const char *source;
  smith_position_t position;
} smith_cursor_t;
