
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
  char * serialized = RL_SAVE_CINIT (rl_td_t, &RL_DESCRIPTOR_PREFIX (color_t));
  if (serialized)
    {
      printf ("color_t enum desciptor\n%s\n", serialized);
      RL_FREE (serialized);
    }
  
  printf("BLACK: %i\nRED: %i\nGREEN: %i\nYELLOW: %i\nBLUE: %i\nMAGENTA: %i\nCYAN: %i\nWHITE: %i\n",
         BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE);
  
  character_place_t place = { 'A', WHITE, BLUE };
  serialized = RL_SAVE_CINIT (character_place_t, &place);
  if (serialized)
    {
      printf ("place = %s;\n", serialized);
      RL_FREE (serialized);
    }
  return (EXIT_SUCCESS);
}
