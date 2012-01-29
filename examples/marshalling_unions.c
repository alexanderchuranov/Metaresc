
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "marshalling_unions.h"

int main ()
{
  floating_t fl = { .f = M_PI };
  char * xml = RL_SAVE_XML (floating_t, &fl);

  if (NULL == xml)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s", xml);
      RL_FREE (xml);
    }
  
  return (EXIT_SUCCESS);
}
