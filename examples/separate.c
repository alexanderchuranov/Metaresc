
#include <stdio.h>
#include <stdlib.h>

#include "separate_employee.h"

int
main (int argc, char * argv[])
{
  employee_t employee = create_employee ("John", "Doe", 123456);
  MR_PRINT ((employee_t, &employee, XML));
  return (EXIT_SUCCESS);
}
