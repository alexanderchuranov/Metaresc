
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>

TYPEDEF_STRUCT (example_t,
                (int, a1, [7]),
                (long, a2, [11][3]),
                (double, a3, [5][2][9]),
                (bool, a4, [7][5][3][1]),
                )

int print_array_field_info (mr_td_t * td, char name[])
{
  mr_fd_t const * fdp = mr_get_fd_by_name (td, name);

  if (NULL == fdp)
    {
      fprintf (stderr,
	       "error: can't obtain type information for field '%s'\n",
	       name);
      return (EXIT_FAILURE);
    }

  if (fdp->stype.mr_type != MR_TYPE_ARRAY)
    {
      fprintf (stderr,
	       "error: the '%s' field is not an array\n",
	       name);
      return (EXIT_FAILURE);
    }

  printf("field declaration: %s %s", fdp->stype.type, fdp->name.str);

  int i;
  for (i = 0; i < sizeof (fdp->param.array_param.dim.dim) / sizeof (fdp->param.array_param.dim.dim[0]); ++i)
    {
      printf("[%d]", (int)fdp->param.array_param.dim.dim[i].count);
      if (fdp->param.array_param.dim.dim[i].is_last)
	break;
    }

  printf("\n");

  return (EXIT_SUCCESS);
}

int main ()
{
  char type_name[] = "example_t";
  mr_td_t * td = mr_get_td_by_name (type_name);

  if (NULL == td)
    {
      printf ("error: can't obtain type information for '%s'\n",
	      type_name);
      return (EXIT_FAILURE);
    }
  print_array_field_info(td, "a1");
  print_array_field_info(td, "a2");
  print_array_field_info(td, "a3");
  print_array_field_info(td, "a4");
  return (EXIT_SUCCESS);
}
