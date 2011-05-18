
#include <stdio.h>
#include <stdlib.h>

#include "separate_employee.h"

int
main (int argc, char * argv[])
{
  employee_t employee = create_employee("John", "Doe", 123456);
  
  char * str = RL_SAVE_XML (employee_t, &employee);
  printf ("%s", str);
  RL_FREE (str);
  
  return (EXIT_SUCCESS);
}
