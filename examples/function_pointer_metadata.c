
#include <stdio.h>
#include <stdlib.h>
#include <reslib.h>

TYPEDEF_STRUCT (ops,
  (double, add, (float, int)),
  (double, subtract, (float, int)),
  (double, multiply, (float, int)),
  (double, divide, (float, int))
  )

int print_func_field_signature(rl_fd_t const* fd)
{
  if (fd->rl_type != RL_TYPE_FUNC)
  {
    fprintf(stderr,
            "error: the field '%s' is not a pointer to function\n",
            fd->name);
    return EXIT_FAILURE;
  }
  
  size_t const num_args =
    fd->param.func_param.size / sizeof(fd->param.func_param.data[0]);
  
  printf("%s (*) (", fd->param.func_param.data[0].type);
  
  for (size_t i = 1; i < num_args; ++i)
  {
    if (i > 1)
      printf(", ");
    
    rl_fd_t const* arg_type = &fd->param.func_param.data[i];
    printf("%s", arg_type->type);
  }
  
  puts(")");
  return EXIT_SUCCESS;
}

int main()
{
  char const type_name[] = "ops";
  char const field_name[] = "subtract";
  
  rl_td_t const* td = rl_get_td_by_name(type_name);
  
  if (!td)
  {
    fprintf(stderr,
            "error: can't obtain type information for '%s'\n",
            type_name);
    return EXIT_FAILURE;
  }
  
  rl_fd_t const* fd = rl_get_fd_by_name(td, field_name);
  
  if (!fd)
  {
    fprintf(stderr,
            "error: can't obtain type information for '%s' in '%s'\n",
            field_name, type_name);
    return EXIT_FAILURE;
  }
  
  printf("field '%s' has type name '%s'\n", field_name, fd->type);
  printf("the actual type: ");
  return print_func_field_signature(fd);
}
