#define SMITH_ENABLE_ALLOCATOR_MACROS

#include "smith/finite_allocator.h"
#include <assert.h>

typedef struct {
  smith_allocator_t parent;
  size_t allocations;
} finite_allocator_t;

static void *allocate(void *allocator, size_t size, size_t alignment) {
  assert(allocator != NULL);
  finite_allocator_t *finite_allocator = allocator;
  if (finite_allocator->allocations == 0) {
    return NULL;
  }
  finite_allocator->allocations--;
  smith_allocator_t parent = finite_allocator->parent;
  void *pointer = parent.allocate(parent.state, size, alignment);
  return pointer;
}

static void deallocate(void *allocator, void *pointer) {
  assert(allocator != NULL);
  finite_allocator_t *finite_allocator = allocator;
  smith_allocator_t parent = finite_allocator->parent;
  smith_allocator_deallocate(parent, pointer);
}

static void destroy(void *allocator) {
  assert(allocator != NULL);
  finite_allocator_t *finite_allocator = allocator;
  smith_allocator_t parent = finite_allocator->parent;
  smith_allocator_deallocate(parent, finite_allocator);
  smith_allocator_destroy(parent);
}

smith_finite_allocator_create_result_t
smith_finite_allocator_create(smith_allocator_t parent, size_t allocations) {
  finite_allocator_t *finite_allocator =
      smith_allocator_allocate(parent, finite_allocator_t);
  if (finite_allocator == NULL) {
    return (smith_finite_allocator_create_result_t){};
  }
  *finite_allocator = (finite_allocator_t){parent, allocations};
  return (smith_finite_allocator_create_result_t){
      .allocator = {.allocate = allocate,
                    .deallocate = deallocate,
                    .destroy = destroy,
                    .state = finite_allocator},
      .success = true};
}
