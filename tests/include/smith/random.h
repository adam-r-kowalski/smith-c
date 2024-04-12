#pragma once

#include "smith/allocator.h"
#include "smith/string.h"
#include <stdint.h>

int32_t smith_random_uniform(int32_t min, int32_t max);

smith_string_t smith_random_string(smith_allocator_t allocator, int32_t length);
