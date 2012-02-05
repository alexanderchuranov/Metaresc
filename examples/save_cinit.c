
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
  char * employee_serialized = MR_SAVE_CINIT (employee_t, &employee);
  
  if (NULL == employee_serialized)
      printf("Serialization error has occurred.\n");
  else
    {
      printf ("%s\n", employee_serialized);
      MR_FREE (employee_serialized);
    }
  return (employee_serialized ? EXIT_SUCCESS : EXIT_FAILURE);
}
