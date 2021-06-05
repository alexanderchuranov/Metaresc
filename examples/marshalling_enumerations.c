
#include <stdio.h>
#include <stdlib.h>

#include "marshalling_enumerations.h"

int main ()
{
  screen_t screen = {
    .width = 80,
    .height = 25,
    .bg = BLACK,
    .fg = WHITE
  };

  MR_PRINT ((screen_t, &screen, XML));
  return (EXIT_SUCCESS);
}
