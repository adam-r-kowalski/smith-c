#pragma once

#include "smith/allocator.h"

typedef struct {
  smith_allocator_t allocator;
  bool success;
} smith_finite_allocator_create_result_t;

smith_finite_allocator_create_result_t
smith_finite_allocator_create(smith_allocator_t parent, size_t allocations);
