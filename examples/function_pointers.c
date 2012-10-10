
#include <stdio.h>
#include <stdlib.h>

#include <metaresc.h>

TYPEDEF_STRUCT (with_f_pointers_t,
                (char *, pfunc, (int))
                )

int main ()
{
  char * serialized = MR_SAVE_CINIT (mr_td_t, mr_get_td_by_name ("with_f_pointers_t"));
  if (NULL == serialized)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s\n", serialized);
      MR_FREE (serialized);
    }
  return (EXIT_SUCCESS);
}
