#include <stdlib.h>
#include <math.h>
#include <check.h>

#include <reslib.h>

#include <regression.h>

Suite * suite = NULL;

int main (int argc, char * argv[])
{
  int number_failed;
  SRunner * srunner = srunner_create (suite);
  srunner_run_all (srunner, CK_ENV);
  number_failed = srunner_ntests_failed (srunner);
  srunner_free (srunner);
  return ((number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}
