
#include <stdio.h>
#include <stdlib.h>
#include <reslib.h>

TYPEDEF_STRUCT (employee_t,
		(char *, firstname),
		(char *, lastname),
		(int, salary),
		)

void print_employee_info (employee_t const * employee)
{
  printf ("employee:\n"
	  "\tfirst name: %s\n"
	  "\tlast name: %s\n"
	  "\tsalary: %u\n",
	  employee->firstname, employee->lastname, employee->salary);
}

int main ()
{
  employee_t employee = { "John", "Smith", 7350 };
  
  char type_name[] = "employee_t";
  char field_name[] = "salary";
  
  rl_td_t * td = rl_get_td_by_name (type_name);
  
  if (NULL == td)
    {
      printf("error: can't obtain type information for '%s'\n", type_name);
      return (EXIT_FAILURE);
    }
  
  rl_fd_t * fd = rl_get_fd_by_name (td, field_name);
  
  if (NULL == fd)
    {
      printf("error: can't obtain type information for field '%s' in '%s'\n",
	     field_name, type_name);
      return (EXIT_FAILURE);
    }
  
  size_t const offset = fd->offset;
  
  printf ("the value of field '%s' in variable employee is: %i\n",
	  field_name, *(int*)(((char*)&employee) + offset));
  
  printf ("setting field '%s' in variable employee...\n", field_name);
  *(int*)(((char*)&employee) + offset) = 9170;
  
  puts ("printing variable employee...");
  print_employee_info (&employee);
  
  return (EXIT_SUCCESS);
}
