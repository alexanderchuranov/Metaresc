
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>
#include <mr_ic.h>

TYPEDEF_ENUM (color_t,
              (BLACK, = 17),
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
  char name[] = "color_t";
  mr_td_t * tdp = mr_get_td_by_name (name);
  int i, count;
  
  if (NULL == tdp)
    {
      fprintf (stderr,
	       "error: can't obtain type information for '%s'\n",
	       name);
      return (EXIT_FAILURE);
    }
  
  count = tdp->fields.size / sizeof (tdp->fields.data[0]);
  for (i = 0; i < count; ++i)
    {
      mr_fd_t * fdp = tdp->fields.data[i].fdp;
      printf ("%d: %s = %" PRId64 "\n", i, fdp->name.str, fdp->param.enum_value);
    }  
  return (EXIT_SUCCESS);
}
