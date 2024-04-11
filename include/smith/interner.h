#pragma once

#include "smith/string.h"

typedef size_t smith_interned_t;

typedef struct {
  bool success;
  smith_interned_t interned;
} smith_intern_result_t;

typedef struct {
  bool success;
  smith_string_t string;
} smith_lookup_result_t;

typedef struct {
  void *interner;
  smith_intern_result_t (*intern)(void *interner, smith_string_t string);
  smith_lookup_result_t (*lookup)(const void *interner,
                                  smith_interned_t interned);
  void (*destroy)(void *interner);
} smith_interner_t;

void smith_interner_destroy(smith_interner_t interner);
