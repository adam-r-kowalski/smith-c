#define SMITH_ENABLE_ALLOCATOR_MACROS

#include "smith/hash_interner.h"

typedef struct {
  smith_allocator_t allocator;
} smith_hash_interner_t;

static smith_intern_result_t intern(void *interner, smith_string_t string) {
  return (smith_intern_result_t){};
}

static smith_lookup_result_t lookup(const void *interner,
                                    smith_interned_t interned) {
  return (smith_lookup_result_t){};
}

static void destroy(void *interner) {
  smith_hash_interner_t *hash_interner = (smith_hash_interner_t *)interner;
}

smith_interner_t smith_hash_interner_create(smith_allocator_t allocator) {
  smith_hash_interner_t *hash_interner =
      smith_allocator_allocate(allocator, smith_hash_interner_t);
  *hash_interner = (smith_hash_interner_t){.allocator = allocator};
  return (smith_interner_t){.intern = intern,
                            .lookup = lookup,
                            .destroy = destroy,
                            .interner = hash_interner};
}
