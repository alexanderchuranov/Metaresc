
#include <stdio.h>
#include <stdlib.h>

#include "suffixes_comments_userdata.h"

int main ()
{
  char type[] = "example_t";
  mr_td_t * td = mr_get_td_by_name (type);
  if (NULL == td)
    printf ("error: can't obtain type information for type '%s'\n", type);
  else
    MR_PRINT ("Type desciptor for type '", type, "' is\n", (mr_td_t, td));
  return (EXIT_SUCCESS);
}
