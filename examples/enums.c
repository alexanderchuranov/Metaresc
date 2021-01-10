
#include <stdio.h>
#include <stdlib.h>

#include "enums.h"

TYPEDEF_STRUCT (character_place_t,
                char c,
                (color_t, fg),
                (color_t, bg)
                )

int main ()
{
  char * serialized = MR_SAVE_CINIT (mr_td_t, &MR_DESCRIPTOR_PREFIX (color_t));
  if (serialized)
    {
      printf ("color_t enum desciptor\n%s\n", serialized);
      MR_FREE (serialized);
    }

  printf("RED: %i\nORANGE: %i\nYELLOW: %i\nGREEN: %i\nBLUE: %i\nPURPLE: %i\nPINK: %i\nBROWN: %i\n",
         RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, PINK, BROWN);

  character_place_t place = { 'A', RED, BROWN };
  serialized = MR_SAVE_CINIT (character_place_t, &place);
  if (serialized)
    {
      printf ("place = %s", serialized);
      MR_FREE (serialized);
    }
  return (EXIT_SUCCESS);
}
