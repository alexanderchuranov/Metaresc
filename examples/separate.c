
#include <stdio.h>
#include <stdlib.h>

#include "separate_employee.h"

int
main (int argc, char * argv[])
{
  employee_t employee = create_employee ("John", "Doe", 123456);
  
  char * employee_xml = RL_SAVE_XML (employee_t, &employee);
  if (employee_xml)
    {
      printf ("%s", employee_xml);
      RL_FREE (employee_xml);
    }
  
  return (EXIT_SUCCESS);
}
