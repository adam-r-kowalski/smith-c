#include "smith/interner.h"

smith_intern_result_t smith_interner_intern(smith_interner_t interner,
                                            smith_string_t string) {
  return interner.intern(interner.state, string);
}

smith_lookup_result_t smith_interner_lookup(smith_interner_t interner,
                                            smith_interned_t interned) {
  return interner.lookup(interner.state, interned);
}

void smith_interner_destroy(smith_interner_t interner) {
  interner.destroy(interner.state);
}
