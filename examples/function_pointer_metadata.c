
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>

TYPEDEF_STRUCT (ops,
		(double, add, (float, int)),
		(double, subtract, (float, int)),
		(double, multiply, (float, int)),
		(double, divide, (float, int))
		)

int print_func_field_signature (mr_fd_t const * fd)
{
  int i;
  
  if (fd->mr_type != MR_TYPE_FUNC)
    {
      fprintf(stderr,
	      "error: the field '%s' is not a pointer to function\n",
	      fd->hashed_name.name);
      return (EXIT_FAILURE);
    }
  
  size_t const num_args = fd->param.func_param.size / sizeof (fd->param.func_param.data[0]);
  
  printf ("%s (*) (", fd->param.func_param.data[0].type);
  
  for (i = 1; i < num_args; ++i)
    {
      if (i > 1)
	printf(", ");
    
      mr_fd_t const* arg_type = &fd->param.func_param.data[i];
      printf ("%s", arg_type->type);
    }
  
  printf (")\n");
  return (EXIT_SUCCESS);
}

int main ()
{
  char type_name[] = "ops";
  char field_name[] = "subtract";
  
  mr_td_t * td = mr_get_td_by_name (type_name);
  
  if (NULL == td)
    {
      fprintf (stderr,
	       "error: can't obtain type information for '%s'\n",
	       type_name);
      return (EXIT_FAILURE);
    }
  
  mr_fd_t const * fd = mr_get_fd_by_name (td, field_name);
  
  if (NULL == fd)
    {
      fprintf (stderr,
	       "error: can't obtain type information for '%s' in '%s'\n",
	       field_name, type_name);
      return (EXIT_FAILURE);
    }
  
  printf ("field '%s' has type name '%s'\n", field_name, fd->type);
  printf ("the actual type: ");
  return (print_func_field_signature (fd));
}
