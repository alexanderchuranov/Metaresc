
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>

TYPEDEF_STRUCT (employee_t,
		(char *, firstname),
		(char *, lastname),
		(int, salary),
		)

int
main (int argc, char * argv[])
{
  employee_t employee = { "John", "Doe", 123456 };
  MR_PRINT ((employee_t, &employee, SCM), "\n");
  return (EXIT_SUCCESS);
}
