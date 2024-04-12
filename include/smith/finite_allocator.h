#pragma once

#include "smith/allocator.h"

smith_allocator_t smith_finite_allocator_create(smith_allocator_t parent,
                                                size_t allocations);
