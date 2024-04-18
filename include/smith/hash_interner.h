#pragma once

#include "smith/allocator.h"
#include "smith/interner.h"

/**
 * Defines the minimum capacity for the hash interner.
 */
#define SMITH_HASH_INTERNER_MIN_CAPACITY 8

/**
 * Defines the growth factor used when resizing the hash table in the interner.
 */
#define SMITH_HASH_INTERNER_GROWTH_FACTOR 2

/**
 * Structure representing the result of creating a hash-based string interner.
 *
 * @param interner The created hash interner.
 * @param success Indicates whether the creation was successful.
 */
typedef struct {
  smith_interner_t interner;
  bool success;
} smith_hash_interner_create_result_t;

/**
 * Creates a hash-based string interner using the specified allocator.
 * The hash interner ensures that each string is stored only once,
 * and provides efficient lookups.
 *
 * @param allocator The allocator to use for memory management in the interner.
 * @return The result of creating the hash interner.
 */
smith_hash_interner_create_result_t
smith_hash_interner_create(smith_allocator_t allocator);
