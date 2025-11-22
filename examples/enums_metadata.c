
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>
#include <mr_ic.h>

TYPEDEF_ENUM (color_t,
              (BLACK,   = 8),
              (RED),
              (GREEN,   = 2),
              (YELLOW,  = 3),
              (BLUE,    = 4),
              MAGENTA,
              CYAN,
              WHITE
              )

int main ()
{
  mr_conf_init ();

  char * type_name = "color_t";
  mr_td_t * tdp = mr_get_td_by_name (type_name);

  if (NULL == tdp)
    {
      fprintf (stderr,
	       "error: can't obtain type information for '%s'\n",
	       type_name);
      return (EXIT_FAILURE);
    }

  int i;
  for (i = 0; i < tdp->param.enum_param.enums_count; ++i)
    {
      mr_ed_t * edp = tdp->param.enum_param.enums[i];
      printf ("%d: %s = %" PRIu64 "\n", i, edp->name.str, edp->value._unsigned);
    }
  return (EXIT_SUCCESS);
}
