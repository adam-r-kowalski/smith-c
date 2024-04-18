#pragma once

#include "smith/allocator.h"

/**
 * Creates an allocator that does not perform any allocation.
 * This allocator returns NULL for every allocation attempt.
 * It can be useful for testing error handling in situations where memory cannot be allocated.
 *
 * @return An allocator that never allocates memory.
 */
smith_allocator_t smith_null_allocator_create();
