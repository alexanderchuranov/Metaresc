
#include <stdio.h>
#include <stdlib.h>

#include <reslib.h>

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
  mr_td_t * td = mr_get_td_by_name (name);
  int i;
  
  if (NULL == td)
    {
      fprintf (stderr,
	       "error: can't obtain type information for '%s'\n",
	       name);
      return (EXIT_FAILURE);
    }
  
  size_t const num_constants = td->fields.size / sizeof (td->fields.data[0]);
  
  for (i = 0; i < num_constants; ++i)
    {
      mr_fd_t const * fd = &td->fields.data[i];
      printf ("%d: %s = %" PRId64 "\n", i, fd->name, fd->param.enum_value);
    }
  
  return (EXIT_SUCCESS);
}
