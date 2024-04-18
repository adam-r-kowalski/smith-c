#pragma once

#include "smith/allocator.h"

/**
 * A structure representing the result of creating a finite allocator.
 * Contains a flag indicating whether the creation was successful and the allocator instance.
 *
 * @param allocator The created allocator.
 * @param success Boolean indicating whether the allocator was successfully created.
 */
typedef struct {
  smith_allocator_t allocator;
  bool success;
} smith_finite_allocator_create_result_t;

/**
 * Creates a finite allocator that allows only a specified number of allocations.
 * After the limit is reached, the allocator returns NULL for further allocation attempts.
 * This is useful for testing memory allocation failure scenarios.
 *
 * @param parent The parent allocator to use for allocating memory.
 * @param allocations The maximum number of allocations the allocator should permit.
 * @return A result containing the new allocator and a success flag.
 */
smith_finite_allocator_create_result_t
smith_finite_allocator_create(smith_allocator_t parent, size_t allocations);
