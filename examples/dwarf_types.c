#include <metaresc.h>

static mr_dwarf_t mr_dwarf =
#include "dwarf_types.h"
  ;

static inline void __attribute__((constructor)) dwarf_types (void) { mr_add_dwarf (&mr_dwarf); }
