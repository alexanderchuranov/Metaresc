#include <metaresc.h>

static mr_td_t mr_td[] = {
#include "dwarf_types.h"
};

static inline void __attribute__((constructor)) dwarf_types (void)
{
  int i;
  for (i = sizeof (mr_td) / sizeof (mr_td[0]) - 1; i >= 0; --i)
    mr_add_type (&mr_td[i], "", NULL);
}
