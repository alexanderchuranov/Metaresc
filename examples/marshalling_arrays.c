
#include <stdio.h>
#include <stdlib.h>

#include "marshalling_arrays.h"

int main ()
{
  array_t a = { .a = { 11, 17, 23 } };
  array2d_t a2d = { { {1, 2}, {3, 4} } };
  char * xml = MR_SAVE_XML (array_t, &a);

  if (NULL == xml)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s", xml);
      MR_FREE (xml);
    }
  
  xml = MR_SAVE_XML (array2d_t, &a2d);

  if (NULL == xml)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s", xml);
      MR_FREE (xml);
    }
  
  return (EXIT_SUCCESS);
}
