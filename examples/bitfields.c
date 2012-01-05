
#include <stdio.h>
#include "bitfields.h"

int main()
{
  printf("size: %zd\n", sizeof(event_coordinates_t));
  event_coordinates_t coords;
  memset(&coords, 0, sizeof(coords));
  puts(RL_SAVE_JSON(event_coordinates_t, &coords));
  return 0;
}
