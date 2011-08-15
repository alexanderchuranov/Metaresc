
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
  char const name[] = "employee_t";
  rl_td_t const* td = rl_get_td_by_name(name);
  
  if (!td)
  {
    printf("error: can't obtain type information for '%s'\n", name);
    return EXIT_FAILURE;
  }
  
  size_t const num_fields = td->fields.size / sizeof(td->fields.data[0]);
  
  printf("the '%s' structure has %u fields:\n", name, num_fields);
  
  for (size_t i = 0; i < num_fields; ++i)
  {
    rl_fd_t const* fd = &td->fields.data[i];
    
    printf("\t%u: name = %s, type = %s, size = %u bytes\n",
           i, fd->name, fd->type, fd->size);
  }
  
  return EXIT_SUCCESS;
}
