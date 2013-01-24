
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>

TYPEDEF_STRUCT (example_t,
                (char*, str),
                (int, a1, [7]),
                (long, a2, [11][3]),
                (double, a3, [5][2][9])
                )

int print_array_field_info (mr_td_t * td, char name[])
{
  mr_fd_t const * fd = mr_get_fd_by_name (td, name);
  
  if (NULL == fd)
    {
      fprintf (stderr,
	       "error: can't obtain type information for field '%s'\n",
	       name);
      return (EXIT_FAILURE);
    }
  
  if (fd->mr_type_ext != MR_TYPE_EXT_ARRAY)
    {
      fprintf (stderr,
	       "error: the '%s' field is not an array\n",
	       name);
      return (EXIT_FAILURE);
    }
  
  size_t const total_items = fd->param.array_param.count;
  size_t const rows = fd->param.array_param.row_count;
  
  printf("field declaration: %s %s", fd->type, fd->name.name);
  
  if (rows == 1) // 1-dimensional
    printf("[%zd]\n", total_items);
  else  // 2- or more dimensional
    printf("[%zd][%zd]\n", total_items / rows, rows);
  
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
  return (EXIT_SUCCESS);
}
