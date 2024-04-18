#pragma once

#include <stddef.h>

/**
 * Structure that represents a memory allocator.
 * The allocator uses function pointers to specify custom allocation,
 * deallocation, and destruction behaviors.
 *
 * @param state Pointer to the allocator-specific state.
 * @param allocate Function to allocate memory of a specified size and alignment.
 * @param deallocate Function to free previously allocated memory.
 * @param destroy Function to clean up any resources associated with the allocator.
 */
typedef struct {
  void *state;
  void *(*allocate)(void *allocator, size_t size, size_t alignment);
  void (*deallocate)(void *allocator, void *pointer);
  void (*destroy)(void *allocator);
} smith_allocator_t;

/**
 * Deallocates memory using the specified allocator.
 *
 * @param allocator The allocator to use for deallocation.
 * @param pointer Pointer to the memory to be deallocated.
 */
void smith_allocator_deallocate(smith_allocator_t allocator, void *pointer);

/**
 * Destroys the allocator, cleaning up any internal resources.
 *
 * @param allocator The allocator to be destroyed.
 */
void smith_allocator_destroy(smith_allocator_t allocator);

#ifdef SMITH_ENABLE_ALLOCATOR_MACROS
// Macros to simplify allocation and deallocation using the allocator's functions.

/**
 * Allocates memory for a single instance of type `type` using the specified allocator.
 */
#define smith_allocator_allocate(allocator, type)                              \
  (type *)(allocator.allocate)(allocator.state, sizeof(type), alignof(type))

/**
 * Allocates memory for an array of `count` elements of type `type`, using the specified allocator.
 */
#define smith_allocator_allocate_array(allocator, type, count)                 \
  (type *)(allocator.allocate)(allocator.state, sizeof(type) * count,          \
                               alignof(type))

#endif
