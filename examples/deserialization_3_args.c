
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
  char const source[] = "<?xml version=\"1.0\"?>"
                        "<employee>"
                          "<firstname>John</firstname>"
                          "<lastname>Doe</lastname>"
                          "<salary>123456</salary>"
                        "</employee>";
  
  employee_t employee;
  
  if (RL_LOAD_XML( employee_t, source, &employee ))
  {
    printf ("%11s: %s\n"
            "%11s: %s\n"
            "%11s: %u\n",
            
            "firstname", employee.firstname,
            "lastname",  employee.lastname,
            "salary",    employee.salary);
    
    return (EXIT_SUCCESS);
  }
  else {
    fprintf(stderr, "De-serialization error has occurred.\n");
    return (EXIT_FAILURE);
  }
}
