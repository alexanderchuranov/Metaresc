
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <basic.h>

int
main (int argc, char * argv[])
{
  employee_t employee;
  
  employee.firstname = "John";
  employee.lastname  = "Doe";
  employee.salary    = 123456;
  
  char * str = RL_SAVE_XML (employee_t, &employee);
  printf ("%s\n", str);
  RL_FREE (str);
  
  return (EXIT_SUCCESS);
}
