
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
  char scm[] =
    "(                        \n"
    "  (\"John\" . firstname) \n"
    "  (\"Doe\" . lastname)   \n"
    "  (123456 . salary))     \n"
    ;
  
  employee_t e = RL_LOAD_SCM (employee_t, scm );
  
  print_info(&e);
  
  char corrupted[] = "this is not a Scheme language text";
  
  if (RL_LOAD_SCM (employee_t, corrupted, &e))
  {
    print_info(&e);
  }
  else {
    fprintf(stderr, "SCM parsing error!\n");
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
