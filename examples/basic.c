
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <basic.h>
#include <basic.h> // included second time for a reason

int
main (int argc, char * argv[])
{
  employee_t employee;
  
  employee.firstname = strdup ("John");
  employee.lastname  = strdup ("Smith");
  employee.salary    = 123456;
  
  char* str = RL_SAVE_XML (employee_t, &employee);
  printf ("%s\n", str);
  RL_FREE (str);
  
  return (EXIT_SUCCESS);
}
