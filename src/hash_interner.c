#define SMITH_ENABLE_ALLOCATOR_MACROS

#include "smith/hash_interner.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>

typedef struct {
  smith_allocator_t allocator;
  smith_string_t *strings;
  uint64_t *hashes;
  bool *occupied;
  size_t capacity;
  size_t count;
} smith_hash_interner_t;

static uint64_t hash(smith_string_t string) {
  uint64_t hash = 0;
  for (size_t i = 0; i < string.length; i++) {
    hash = hash * 31 + string.data[i];
  }
  return hash;
}

static size_t max(size_t a, size_t b) { return a > b ? a : b; }

static bool grow_if_needed(smith_hash_interner_t *interner) {
  if (interner->count < interner->capacity)
    return true;
  size_t new_capacity =
      max(interner->capacity * SMITH_HASH_INTERNER_GROWTH_FACTOR,
          SMITH_HASH_INTERNER_MIN_CAPACITY);
  smith_allocator_t allocator = interner->allocator;
  smith_string_t *strings =
      smith_allocator_allocate_array(allocator, smith_string_t, new_capacity);
  if (strings == nullptr) {
    return false;
  }
  uint64_t *hashes =
      smith_allocator_allocate_array(allocator, uint64_t, new_capacity);
  if (hashes == nullptr) {
    smith_allocator_deallocate(allocator, strings);
    return false;
  }
  bool *occupied =
      smith_allocator_allocate_array(allocator, bool, new_capacity);
  if (occupied == nullptr) {
    smith_allocator_deallocate(allocator, strings);
    smith_allocator_deallocate(allocator, hashes);
    return false;
  }
  memcpy(strings, interner->strings, interner->count * sizeof(smith_string_t));
  memcpy(hashes, interner->hashes, interner->count * sizeof(uint64_t));
  memcpy(occupied, interner->occupied, interner->count * sizeof(bool));
  smith_allocator_deallocate(allocator, interner->strings);
  smith_allocator_deallocate(allocator, interner->hashes);
  smith_allocator_deallocate(allocator, interner->occupied);
  interner->strings = strings;
  interner->hashes = hashes;
  interner->occupied = occupied;
  interner->capacity = new_capacity;
  return true;
}

static smith_intern_result_t intern(void *interner, smith_string_t string) {
  assert(interner != nullptr);
  smith_hash_interner_t *hash_interner = (smith_hash_interner_t *)interner;
  if (!grow_if_needed(hash_interner)) {
    return (smith_intern_result_t){};
  }
  uint64_t hash_value = hash(string);
  for (size_t i = 0; i < hash_interner->capacity; i++) {
    size_t index = (i + hash_value) % hash_interner->capacity;
    if (!hash_interner->occupied[index]) {
      hash_interner->strings[index] = string;
      hash_interner->hashes[index] = hash_value;
      hash_interner->occupied[index] = true;
      hash_interner->count++;
      return (smith_intern_result_t){.success = true, .interned = index};
    }
    smith_string_t existing_string = hash_interner->strings[index];
    bool same_string =
        hash_interner->hashes[index] == hash_value &&
        existing_string.length == string.length &&
        memcmp(existing_string.data, string.data, string.length) == 0;
    if (same_string) {
      return (smith_intern_result_t){.success = true, .interned = index};
    }
  }
  return (smith_intern_result_t){};
}

static smith_lookup_result_t lookup(const void *interner,
                                    smith_interned_t interned) {
  assert(interner != nullptr);
  smith_hash_interner_t *hash_interner = (smith_hash_interner_t *)interner;
  if (!hash_interner->occupied[interned]) {
    return (smith_lookup_result_t){};
  }
  return (smith_lookup_result_t){.success = true,
                                 .string = hash_interner->strings[interned]};
}

static void destroy(void *interner) {
  assert(interner != nullptr);
  smith_hash_interner_t *hash_interner = (smith_hash_interner_t *)interner;
  smith_allocator_t allocator = hash_interner->allocator;
  smith_allocator_deallocate(allocator, hash_interner->strings);
  smith_allocator_deallocate(allocator, hash_interner->hashes);
  smith_allocator_deallocate(allocator, hash_interner->occupied);
}

smith_interner_t smith_hash_interner_create(smith_allocator_t allocator) {
  smith_hash_interner_t *hash_interner =
      smith_allocator_allocate(allocator, smith_hash_interner_t);
  assert(hash_interner != nullptr);
  *hash_interner = (smith_hash_interner_t){.allocator = allocator};
  return (smith_interner_t){.intern = intern,
                            .lookup = lookup,
                            .destroy = destroy,
                            .state = hash_interner};
}
