
#include <stdio.h>
#include "marshalling_pointers_to_functions.h"

int main()
{
  with_functions_t s = {
    .value = 7,
    .func = puts
  };
  
  char * xml = RL_SAVE_XML (with_functions_t, &s);
  puts (xml);
  RL_FREE (xml);
  
  return 0;
}
