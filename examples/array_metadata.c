
#include <stdio.h>
#include <stdlib.h>
#include <reslib.h>

TYPEDEF_STRUCT (example_t,
                (char*, str),
                (int, a1, [7]),
                (long, a2, [11][3]),
                (double, a3, [5][2][9])
                )

int print_array_field_info(rl_td_t * td, char name[])
{
  rl_fd_t const* fd = rl_get_fd_by_name(td, name);
  
  if (!fd)
  {
    fprintf(stderr,
            "error: can't obtain type information for field '%s'\n",
            name);
    return EXIT_FAILURE;
  }
  
  if (fd->rl_type_ext != RL_TYPE_EXT_ARRAY)
  {
    fprintf(stderr,
            "error: the '%s' field is not an array\n",
            name);
    return EXIT_FAILURE;
  }
  
  size_t const total_items = fd->param.array_param.count;
  size_t const rows = fd->param.array_param.row_count;
  
  printf("field declaration: %s %s", fd->type, fd->name);
  
  if (rows == 1) // 1-dimensional
  {
    printf("[%zd]\n", total_items);
  }
  else { // 2- or more dimensional
    printf("[%zd][%zd]\n", total_items / rows, rows);
  }
  
  return EXIT_SUCCESS;
}

int main()
{
  char type_name[] = "example_t";
  rl_td_t * td = rl_get_td_by_name(type_name);
  
  if (!td)
  {
    fprintf(stderr,
            "error: can't obtain type information for '%s'\n",
            type_name);
    return EXIT_FAILURE;
  }
  
  return
    print_array_field_info(td, "a1") == EXIT_SUCCESS
    && print_array_field_info(td, "a2") == EXIT_SUCCESS
    && print_array_field_info(td, "a3") == EXIT_SUCCESS;
}
