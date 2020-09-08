#include <metaresc.h>

static mr_td_t mr_td[] = {
#include "dwarf_types.h"
};

static inline void __attribute__((constructor)) dwarf_types (void)
{
  int i, count = sizeof (mr_td) / sizeof (mr_td[0]);
  for (i = 0; i < count; ++i)
    mr_add_type (&mr_td[i], "", NULL);
}
