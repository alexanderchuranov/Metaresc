
#include <stdio.h>
#include <stdlib.h>

#include "simplified_grammar.h"

int main ()
{
  event_t ev = { .x = 11, .y = 17, .z = 31 };
  MR_PRINT ("event_t ev = ", (event_t, &ev));
  return (EXIT_SUCCESS);
}
