
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>

TYPEDEF_STRUCT (employee_t,
		(char *, firstname),
		(char *, lastname),
		(int, salary),
		)

void print_info (employee_t const * e)
{
  printf ("First name: %s\nLast name: %s\nSalary: %u\n",
	  e->firstname, e->lastname, e->salary);
}

int main (int argc, char * argv[])
{
  char serialized[] =
    "(                        \n"
    "  (\"John\" . firstname) \n"
    "  (\"Doe\" . lastname)   \n"
    "  (123456 . salary))     \n"
    ;
  
  employee_t employee = MR_LOAD_SCM (employee_t, serialized);
  
  print_info (&employee);
  MR_FREE_RECURSIVELY (employee_t, &employee);
  
  char corrupted[] = "this is not a Scheme language text";
  
  if (MR_LOAD_SCM (employee_t, corrupted, &employee))
    printf ("Parsing successfull (unexpected behavior)\n");
  else 
    printf ("Parsing error (expected behavior)\n");
  
  return (EXIT_SUCCESS);
}
