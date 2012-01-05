
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
  char xml[] =
    "<?xml version='1.0'?>"
    "<employee>"
      "<firstname>John</firstname>"
      "<lastname>Doe</lastname>"
      "<salary>123456</salary>"
    "</employee>";
  
  employee_t e = RL_LOAD_XML (employee_t, xml);
  
  printf(
    "First name: %s\nLast name: %s\nSalary: %u\n",
    e.firstname, e.lastname, e.salary);
  
  return EXIT_SUCCESS;
}
