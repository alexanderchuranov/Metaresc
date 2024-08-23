#include <metaresc.h>

static mr_dwarf_t mr_dwarf[] = {
#include <dw_types.h>
};

static void __attribute__ ((constructor)) dw_types_init () 
{
  int i, count = sizeof (mr_dwarf) / sizeof (mr_dwarf[0]);
  for (i = 0; i < count; ++i)
    mr_add_dwarf (&mr_dwarf[i]);
}
