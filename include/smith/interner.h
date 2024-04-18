#pragma once

#include "smith/string.h"

/**
 * Represents an identifier for interned strings.
 */
typedef size_t smith_interned_t;

/**
 * Result structure for interning operations.
 *
 * @param success Indicates whether the interning was successful.
 * @param interned The identifier for the interned string.
 */
typedef struct {
  bool success;
  smith_interned_t interned;
} smith_intern_result_t;

/**
 * Result structure for lookup operations.
 *
 * @param success Indicates whether the lookup was successful.
 * @param string The string associated with the identifier.
 */
typedef struct {
  bool success;
  smith_string_t string;
} smith_lookup_result_t;

/**
 * Structure that represents a string interner.
 * The interner is responsible for managing a set of unique strings.
 *
 * @param state Pointer to the interner-specific state.
 * @param intern Function to intern a string.
 * @param lookup Function to lookup a string by its identifier.
 * @param destroy Function to destroy the interner and clean up resources.
 */
typedef struct {
  void *state;
  smith_intern_result_t (*intern)(void *interner, smith_string_t string);
  smith_lookup_result_t (*lookup)(const void *interner, smith_interned_t interned);
  void (*destroy)(void *interner);
} smith_interner_t;

/**
 * Interns a string using the specified interner.
 *
 * @param interner The interner to use.
 * @param string The string to intern.
 * @return The result of the interning operation.
 */
smith_intern_result_t smith_interner_intern(smith_interner_t interner, smith_string_t string);

/**
 * Looks up a string by its identifier in the specified interner.
 *
 * @param interner The interner to use.
 * @param interned The identifier of the string to look up.
 * @return The result of the lookup operation.
 */
smith_lookup_result_t smith_interner_lookup(smith_interner_t interner, smith_interned_t interned);

/**
 * Destroys the interner, releasing all associated resources.
 *
 * @param interner The interner to destroy.
 */
void smith_interner_destroy(smith_interner_t interner);
