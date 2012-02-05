
#include <stdio.h>
#include <stdlib.h>

#include "marshalling_arrays.h"

int main ()
{
  with_array_t wa = { .a = { 11, 17, 23 } };
  char * xml = MR_SAVE_XML (with_array_t, &wa);

  if (NULL == xml)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s", xml);
      MR_FREE (xml);
    }
  
  return (EXIT_SUCCESS);
}
