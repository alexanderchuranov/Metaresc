
#include <stdio.h>
#include <stdlib.h>

#include "marshalling_bit_fields.h"

int main ()
{
  event_coordinates_t coords = {
    .longitude = 51.5,
    .latitude  = 46.0,
    .year = 2011,
    .month = 6,
    .day = 29,
    .hour = 10,
    .minute = 25,
    .second = 31
  };
  
  char * xml = MR_SAVE_XML (event_coordinates_t, &coords);

  if (NULL == xml)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s", xml);
      MR_FREE (xml);
    }
  
  return (EXIT_SUCCESS);
}
