#include "smith/system_allocator.h"
#include <stdlib.h>

static void *allocate(void *allocator, size_t size, size_t alignment) {
  return malloc(size);
}

static void deallocate(void *allocator, void *pointer) { free(pointer); }

static void destroy(void *allocator) {}

smith_allocator_t smith_system_allocator_create() {
  return (smith_allocator_t){.allocate = allocate,
                             .deallocate = deallocate,
                             .destroy = destroy,
                             .allocator = NULL};
}
