
#include <stdio.h>
#include <stdlib.h>

#include "simplified_grammar.h"

int main ()
{
  event_t ev = { .x = 11, .y = 17, .z = 31 };
  char * ev_serialized = RL_SAVE_CINIT (event_t, &ev);
  
  if (ev_serialized)
    {
      printf ("event_t ev = %s;\n", ev_serialized);
      RL_FREE (ev_serialized);
    }
  
  return (EXIT_SUCCESS);
}
