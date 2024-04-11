#include "smith/interner.h"

void smith_interner_destroy(smith_interner_t interner) {
  interner.destroy(interner.interner);
}
