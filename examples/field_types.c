
#include <stdio.h>
#include <stdlib.h>
#include <reslib.h>

TYPEDEF_STRUCT (employee_t,
		(char *, firstname),
		(char *, lastname),
		(int, salary),
		)

int main()
{
  char name[] = "employee_t";
  rl_td_t * td = rl_get_td_by_name(name);
  int i;
  
  if (!td)
  {
    printf("error: can't obtain type information for '%s'\n", name);
    return EXIT_FAILURE;
  }
  
  int const num_fields = td->fields.size / sizeof(td->fields.data[0]);
  
  printf("the '%s' structure has %u fields:\n", name, num_fields);
  
  for (i = 0; i < num_fields; ++i)
  {
    rl_fd_t const* fd = &td->fields.data[i];
    
    printf("\t%u: name = %s, type = %s, size = %u bytes\n",
           i, fd->name, fd->type, fd->size);
  }
  
  return EXIT_SUCCESS;
}
