#include "smith/null_allocator.h"

static void *allocate(void *allocator, size_t size, size_t alignment) {
  return nullptr;
}

static void deallocate(void *allocator, void *pointer) {}

static void destroy(void *allocator) {}

smith_allocator_t smith_null_allocator_create() {
  return (smith_allocator_t){.allocate = allocate,
                             .deallocate = deallocate,
                             .destroy = destroy,
                             .state = NULL};
}
