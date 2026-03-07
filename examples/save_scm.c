
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
  MR_PRINT ((employee_t, &employee, SCM), "\n");
  long double ld = 3.14159265358979323846L;
  char * ld_scm = MR_SAVE_SCM (long double, &ld);
  long double ld_ = MR_LOAD_SCM (long double, ld_scm);
  printf ("ld = %.20Lg ld_scm = %s ld_ = %.20Lg\n", ld, ld_scm, ld_);
  return (EXIT_SUCCESS);
}
