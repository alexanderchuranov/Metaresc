
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>

TYPEDEF_STRUCT (ops,
		(double, add, (float, int)),
		(double, subtract, (float, int)),
		(double, multiply, (float, int)),
		(double, divide, (float, int))
		)

int print_func_field_signature (mr_fd_t const * fdp)
{
  int i;

  if (fdp->stype.mr_type != MR_TYPE_FUNC)
    {
      fprintf(stderr,
	      "error: the field '%s' is not a pointer to function\n",
	      fdp->name.str);
      return (EXIT_FAILURE);
    }

  size_t const num_args = fdp->func_param.size / sizeof (fdp->func_param.args[0]);

  printf ("%s (*) (", fdp->func_param.args[0]->type);

  for (i = 1; i < num_args; ++i)
    {
      if (i > 1)
	printf(", ");

      printf ("%s", fdp->func_param.args[i]->type);
    }

  printf (")\n");
  return (EXIT_SUCCESS);
}

int main ()
{
  char type_name[] = "ops";
  char field_name[] = "subtract";

  mr_conf_init ();

  mr_td_t * td = mr_get_td_by_name (type_name);

  if (NULL == td)
    {
      fprintf (stderr,
	       "error: can't obtain type information for '%s'\n",
	       type_name);
      return (EXIT_FAILURE);
    }

  mr_fd_t const * fdp = mr_get_fd_by_name (td, field_name);

  if (NULL == fdp)
    {
      fprintf (stderr,
	       "error: can't obtain type information for '%s' in '%s'\n",
	       field_name, type_name);
      return (EXIT_FAILURE);
    }

  printf ("field '%s' has type name '%s'\n", field_name, fdp->stype.type);
  printf ("the actual type: ");
  return (print_func_field_signature (fdp));
}
