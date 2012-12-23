
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>
#include <mr_ic.h>

TYPEDEF_STRUCT (employee_t,
		(char *, firstname),
		(char *, lastname),
		(int, salary),
		)

static int
print_fd (mr_ptr_t key, const void * context)
{
  mr_fd_t * fdp = key.ptr;
  int * i = (void*)context;
  printf("\t%u: name = %s, type = %s, size = %u bytes\n",
	 (*i)++, fdp->hashed_name.name, fdp->type, fdp->size);
  return (0);
}

int main ()
{
  char name[] = "employee_t";
  mr_td_t * td = mr_get_td_by_name(name);
  int i = 0;
  
  if (NULL == td)
    {
      printf ("error: can't obtain type information for '%s'\n", name);
      return (EXIT_FAILURE);
    }
  
  mr_ic_foreach (&td->fields, print_fd, &i);
  
  return (EXIT_SUCCESS);
}
