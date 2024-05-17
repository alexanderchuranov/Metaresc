
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>
#include <mr_ic.h>

TYPEDEF_STRUCT (employee_t,
		(char *, firstname),
		(char *, lastname),
		(int, salary),
		)

int main ()
{
  char name[] = "employee_t";
  mr_td_t * tdp = mr_get_td_by_name(name);
  int i, count;

  if (NULL == tdp)
    {
      printf ("error: can't obtain type information for '%s'\n", name);
      return (EXIT_FAILURE);
    }

  count = tdp->param.struct_param.fields_size / sizeof (tdp->param.struct_param.fields[0]);
  for (i = 0; i < count; ++i)
    {
      mr_fd_t * fdp = tdp->param.struct_param.fields[i];
      printf("\t%d: name = %s, type = %s, size = %u bytes\n",
	     i, fdp->name.str, fdp->stype.type, fdp->stype.size);
    }

  return (EXIT_SUCCESS);
}
