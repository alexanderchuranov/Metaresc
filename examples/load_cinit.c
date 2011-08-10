
#include <stdio.h>
#include <stdlib.h>

#include <reslib.h>

TYPEDEF_STRUCT (employee_t,
		(char *, firstname),
		(char *, lastname),
		(int, salary),
		)

void print_info(employee_t const* e)
{
  printf(
    "First name: %s\nLast name: %s\nSalary: %u\n",
    e->firstname, e->lastname, e->salary);
}

int main (int argc, char * argv[])
{
  char cinit[] =
    "{"                         "\n"
    "  .firstname = \"John\", " "\n"
    "  .lastname  = \"Doe\",  " "\n"
    "  .salary    = 123456,   " "\n"
    "}"                         "\n";
  
  employee_t e = RL_LOAD_CINIT (employee_t, cinit);
  
  print_info(&e);
  
  char corrupted[] = "this is not a C language text";
  
  if (RL_LOAD_CINIT (employee_t, corrupted, &e))
  {
    print_info(&e);
  }
  else {
    fprintf(stderr, "CINIT parsing error!\n");
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
