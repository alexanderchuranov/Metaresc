
#include <stdio.h>
#include <stdlib.h>

#include "simplified_grammar.h"

int main()
{
  event_t ev = { .x = 11, .y = 17, .z = 31 };
  
  char * str = RL_SAVE_XML (event_t, &ev);
  
  if (str)
  {
    puts (str);
    RL_FREE (str);
  }
  
  return (EXIT_SUCCESS);
}
