#pragma once

#include "smith/allocator.h"

/**
 * Creates a system allocator that uses standard malloc and free for memory management.
 * This allocator does not maintain any state and uses the C standard library functions
 * for allocation and deallocation.
 *
 * @return An allocator that uses malloc and free for memory operations.
 */
smith_allocator_t smith_system_allocator_create();
