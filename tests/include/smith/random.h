#pragma once

#include "smith/allocator.h"
#include "smith/string.h"

smith_string_t smith_random_string(smith_allocator_t allocator);

smith_string_t smith_random_symbol(smith_allocator_t allocator);

smith_string_t smith_random_int(smith_allocator_t allocator);
