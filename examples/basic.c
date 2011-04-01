
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <basic.h>
#include <basic.h> // included second time for a reason

void read_string(char** dest, char const prompt[])
{
  char s[256];
  
  printf ("%s: ", prompt);
  scanf ("%256s", s);
  
  *dest = malloc (strlen(s) + 1);
  strcpy (*dest, s);
}

void read_uint32(uint32_t* dest, char const prompt[])
{
  printf ("%s: ", prompt);
  scanf ("%d", dest);
}

int
main (int argc, char * argv[])
{
  employee_t employee;
  
  read_string (&employee.firstname, "first name");
  read_string (&employee.lastname, "last name");
  read_uint32 (&employee.salary, "salary");
  
  char* str = RL_SAVE_XML (employee_t, &employee);
  puts ("");
  puts (str);
  RL_FREE (str);
  
  return (EXIT_SUCCESS);
}
