
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
  char xml[] =
    "<?xml version='1.0'?>"
    "<employee>"
      "<firstname>John</firstname>"
      "<lastname>Doe</lastname>"
      "<salary>123456</salary>"
    "</employee>";

  employee_t employee = MR_LOAD_XML (employee_t, xml);

  printf (
	  "First name: %s\nLast name: %s\nSalary: %u\n",
	  employee.firstname, employee.lastname, employee.salary);

  return (EXIT_SUCCESS);
}
