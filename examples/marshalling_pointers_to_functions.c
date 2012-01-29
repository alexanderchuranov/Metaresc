#include <stdio.h>
#include <stdlib.h>

#include "marshalling_pointers_to_functions.h"

int main ()
{
  with_functions_t with_functions = {
    .value = 7,
    .func = puts
  };
  char * xml = RL_SAVE_XML (with_functions_t, &with_functions);
  
  if (NULL == xml)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s", xml);
      RL_FREE (xml);
    }
  
  return (EXIT_SUCCESS);
}
