#pragma once

#include <stddef.h>

typedef struct {
  void *allocator;
  void *(*allocate)(void *allocator, size_t size, size_t alignment);
  void (*deallocate)(void *allocator, void *pointer);
  void (*destroy)(void *allocator);
} smith_allocator_t;

void smith_allocator_deallocate(smith_allocator_t allocator, void *pointer);

void smith_allocator_destroy(smith_allocator_t allocator);

#ifdef SMITH_ENABLE_ALLOCATOR_MACROS

#define smith_allocator_allocate(allocator, type)                              \
  (type *)(allocator.allocate)(allocator.allocator, sizeof(type), alignof(type))

#define smith_allocator_allocate_array(allocator, type, count)                 \
  (type *)(allocator.allocate)(allocator.allocator, sizeof(type) * count,      \
                               alignof(type))

#endif
