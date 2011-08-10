
#include <stdio.h>
#include "enums.h"

TYPEDEF_STRUCT (character_place_t,
                char c,
                (color_t, fg),
                (color_t, bg)
                )

int main()
{
  printf("color_t enum desciptor\n%s\n", RL_SAVE_CINIT (rl_td_t, &RL_DESCRIPTOR_PREFIX (color_t)));
  printf("BLACK: %i\nRED: %i\nGREEN: %i\nYELLOW: %i\nBLUE: %i\nMAGENTA: %i\nCYAN: %i\nWHITE: %i\n",
         BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE);
  puts("");
  
  character_place_t place = { 'A', WHITE, BLUE };
  puts(RL_SAVE_JSON(character_place_t, &place));
  return 0;
}
