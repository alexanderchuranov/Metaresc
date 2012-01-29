#include <stdlib.h>

#include "marshalling_structures_basic_types.h"

int main ()
{
  basics_t b = {
    .c = 'Z',
    .uli = 123,
    .fl = {
      .f = 0.3f,
      .ld = 3.14,
      .dc = 7.42
    }
  };
  char * xml = RL_SAVE_XML (basics_t, &b);
  
  if (NULL == xml)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s", xml);
      RL_FREE (xml);
    }
  return (EXIT_SUCCESS);
}
