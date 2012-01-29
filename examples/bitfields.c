
#include <stdio.h>
#include <stdlib.h>

#include "bitfields.h"

int main ()
{
  printf ("size: %zd\n", sizeof (event_coordinates_t));
  event_coordinates_t coords;
  memset (&coords, 0, sizeof(coords));
  char * xml = RL_SAVE_XML (event_coordinates_t, &coords);

  if (NULL == xml)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s", xml);
      RL_FREE (xml);
    }
  
  return (EXIT_SUCCESS);
}
