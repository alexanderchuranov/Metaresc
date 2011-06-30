
#include <stdio.h>
#include "marshalling_bit_fields.h"

int main()
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
  
  char const* xml = RL_SAVE_SCM( event_coordinates_t, &coords );
  puts(xml);
  RL_FREE( xml );
  
  return 0;
}
