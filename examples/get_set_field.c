
#include <stdio.h>
#include <stdlib.h>
#include <reslib.h>

TYPEDEF_STRUCT (employee_t,
		(char *, firstname),
		(char *, lastname),
		(int, salary),
		)

void print_employee_info(employee_t const* e)
{
  printf(
    "employee:\n"
    "\tfirst name: %s\n"
    "\tlast name: %s\n"
    "\tsalary: %u\n",
    e->firstname, e->lastname, e->salary);
}

int main()
{
  employee_t e = { "John", "Smith", 7350 };
  
  char const type_name[] = "employee_t";
  char const field_name[] = "salary";
  
  rl_td_t const* td = rl_get_td_by_name(type_name);
  
  if (!td)
  {
    printf("error: can't obtain type information for '%s'\n", type_name);
    return EXIT_FAILURE;
  }
  
  rl_fd_t const* fd = rl_get_fd_by_name(td, field_name);
  
  if (!fd)
  {
    printf("error: can't obtain type information for field '%s' in '%s'\n",
           field_name, type_name);
    return EXIT_FAILURE;
  }
  
  size_t const offset = fd->offset;
  
  printf("the value of field '%s' in variable e is: %i\n",
         field_name, *(int*)(((char*)&e) + offset));
  
  printf("setting field '%s' in variable e...\n", field_name);
  *(int*)(((char*)&e) + offset) = 9170;
  
  puts("printing variable e...");
  print_employee_info(&e);
  
  return EXIT_SUCCESS;
}
