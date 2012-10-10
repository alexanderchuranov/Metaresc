
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>

TYPEDEF_FUNC (func_t, int , (char *, int, int *, double), ATTRIBUTES (__attribute__ ((aligned (sizeof (int64_t)))) , "function type sample"))

int main ()
{
  char * serialized = MR_SAVE_CINIT (mr_td_t, mr_get_td_by_name ("func_t"));
  if (NULL == serialized)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s\n", serialized);
      MR_FREE (serialized);
    }
  return (EXIT_SUCCESS);
}
