#pragma once

#include "smith/allocator.h"
#include "smith/interner.h"

smith_interner_t smith_hash_interner_create(smith_allocator_t allocator);
