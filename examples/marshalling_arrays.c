
#include <stdio.h>
#include <stdlib.h>

#include "marshalling_arrays.h"

int main ()
{
  with_array_t wa = { .a = { 11, 17, 23 } };
  char * xml = RL_SAVE_XML (with_array_t, &wa);

  if (NULL == xml)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s", xml);
      RL_FREE (xml);
    }
  
  return (EXIT_SUCCESS);
}
