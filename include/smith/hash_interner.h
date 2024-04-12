#pragma once

#include "smith/allocator.h"
#include "smith/interner.h"

#define SMITH_HASH_INTERNER_MIN_CAPACITY 8
#define SMITH_HASH_INTERNER_GROWTH_FACTOR 2

smith_interner_t smith_hash_interner_create(smith_allocator_t allocator);
