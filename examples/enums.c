
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
  MR_PRINT ("color_t enum desciptor\n", (mr_td_t, &MR_DESCRIPTOR_PREFIX (color_t)));
  MR_PRINT ("RED: ", RED, "\n",
	    "ORANGE: ", ORANGE, "\n",
	    "YELLOW: ", YELLOW, "\n",
	    "GREEN: ", GREEN, "\n",
	    "BLUE: ", BLUE, "\n",
	    "PURPLE: ", PURPLE, "\n",
	    "PINK: ", PINK, "\n",
	    "BROWN: ", BROWN, "\n");

  character_place_t place = { 'A', RED, BROWN };
  MR_PRINT ("place = ", (character_place_t, &place));

  return (EXIT_SUCCESS);
}
