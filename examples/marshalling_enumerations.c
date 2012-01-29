
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
  
  char * xml = RL_SAVE_XML (screen_t, &screen);

  if (NULL == xml)
    printf ("Serialization failed\n");
  else
    {
      printf ("%s", xml);
      RL_FREE (xml);
    }
  
  // TODO: loading unmatched enum value
  return (EXIT_SUCCESS);
}
