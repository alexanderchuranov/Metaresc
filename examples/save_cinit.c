
#include <stdio.h>
#include <stdlib.h>

#include <reslib.h>

TYPEDEF_STRUCT (employee_t,
		(char *, firstname),
		(char *, lastname),
		(int, salary),
		)

int
main (int argc, char * argv[])
{
  employee_t employee;
  
  employee.firstname = "John";
  employee.lastname  = "Doe";
  employee.salary    = 123456;
  
  char * str = RL_SAVE_CINIT (employee_t, &employee);
  
  if (str)
  {
    printf ("%s\n", str);
    RL_FREE (str);
    return (EXIT_SUCCESS);
  }
  else {
    fprintf(stderr, "Serialization error has occurred.\n");
    return (EXIT_FAILURE);
  }
}
