
#include <stdio.h>
#include "enums.h"

int main()
{
  printf("color_t enum desciptor\n%s\n", RL_SAVE_CINIT (rl_td_t, &RL_DESCRIPTOR_PREFIX (color_t)));
  printf("BLACK: %i\nRED: %i\nGREEN: %i\nYELLOW: %i\nBLUE: %i\nMAGENTA: %i\nCYAN: %i\nWHITE: %i\n",
         BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE);
  return 0;
}
