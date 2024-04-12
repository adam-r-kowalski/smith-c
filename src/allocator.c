#include "smith/allocator.h"

void smith_allocator_deallocate(smith_allocator_t allocator, void *pointer) {
  allocator.deallocate(allocator.state, pointer);
}

void smith_allocator_destroy(smith_allocator_t allocator) {
  allocator.destroy(allocator.state);
}
