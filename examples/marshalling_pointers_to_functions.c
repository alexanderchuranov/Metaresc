#include <stdio.h>
#include <stdlib.h>

#include "marshalling_pointers_to_functions.h"

int main ()
{
  with_functions_t with_functions = {
    .value = 7,
    .func = puts
  };
  char * xml = MR_SAVE_XML (with_functions_t, &with_functions);

  if (NULL == xml)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s", xml);
      MR_FREE (xml);
    }

  return (EXIT_SUCCESS);
}
