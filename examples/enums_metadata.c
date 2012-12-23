
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

static int
print_fd (mr_ptr_t key, const void * context)
{
  mr_fd_t * fdp = key.ptr;
  int * i = (void*)context;
  printf ("%d: %s = %" PRId64 "\n", (*i)++, fdp->hashed_name.name, fdp->param.enum_value);
  return (0);
}

int main ()
{
  char name[] = "color_t";
  mr_td_t * td = mr_get_td_by_name (name);
  int i = 0;
  
  if (NULL == td)
    {
      fprintf (stderr,
	       "error: can't obtain type information for '%s'\n",
	       name);
      return (EXIT_FAILURE);
    }
  
  mr_ic_foreach (&td->fields, print_fd, &i);
  
  return (EXIT_SUCCESS);
}
