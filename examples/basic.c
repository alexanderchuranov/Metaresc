
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
  char * xml = MR_SAVE_XML (employee_t, &employee);

  if (NULL == xml)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s", xml);
      MR_FREE (xml);
    }

  return (EXIT_SUCCESS);
}
